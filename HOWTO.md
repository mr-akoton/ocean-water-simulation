# Implementing Ocean Water in OpenGL

This guide explains how to build a real-time ocean water renderer using a **full-screen raymarching** approach, based on the shader code you provided and the wave mathematics from GPU Gems Chapter 1 ("Effective Water Simulation from Physical Models"). The focus is on the math and the systems — since you already have an OpenGL setup, code snippets are illustrative and show the exact functions from the shader.

---

## High-Level Architecture

Before diving in, understand the fundamental difference between two common ocean approaches:

| Approach | GPU Gems Ch. 1 | This Shader |
|---|---|---|
| **Geometry** | Tessellated mesh with vertex displacement | Full-screen quad, no water mesh |
| **Wave evaluation** | Gerstner waves per vertex | Summed exponential-sine octaves per fragment |
| **Intersection** | Implicit (vertices are on the surface) | Raymarched per pixel |
| **Cost** | Scales with mesh density | Scales with fragment count |

The shader you have is a **fragment-only** renderer. Every pixel independently shoots a ray from a camera, finds where it hits the animated water surface via raymarching, and computes lighting there. There is no water mesh at all.

```
[Full-screen quad VAO]
    → vertex shader (passthrough, emits clip-space quad)
    → fragment shader (ray cast → water hit → lighting)
    → framebuffer
```

---

## 1. OpenGL Setup: Full-Screen Quad

You need one VAO/VBO that covers clip space. The vertex shader just passes through:

```glsl
// vertex.glsl
#version 330 core
layout(location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
```

The C++ side:
```cpp
float quadVerts[] = { -1,-1,  1,-1,  1,1,  -1,-1,  1,1,  -1,1 };
// upload to VAO/VBO as usual, bind before each frame's draw call
glDrawArrays(GL_TRIANGLES, 0, 6);
```

### Required Uniforms

The shader needs three uniforms fed from CPU every frame:

```cpp
glUniform2f(glGetUniformLocation(prog, "iResolution"), (float)width, (float)height);
glUniform1f(glGetUniformLocation(prog, "iTime"), (float)glfwGetTime());
glUniform4f(glGetUniformLocation(prog, "iMouse"), mx, my, 0, 0); // last mouse pos
```

---

## 2. Wave Mathematics

### GPU Gems Gerstner Waves vs. This Shader

GPU Gems Chapter 1 defines waves as a sum of **Gerstner waves**, where each wave *i* displaces a surface point both horizontally and vertically:

```
x(u,v,t) = u + Σ  (Qᵢ Aᵢ Dᵢ.x · cos(wᵢ Dᵢ·(u,v) + φᵢ t))
y(u,v,t) =     Σ  (Aᵢ · sin(wᵢ Dᵢ·(u,v) + φᵢ t))
```

where `Dᵢ` is wave direction, `Aᵢ` is amplitude, `wᵢ` is angular frequency, and `Qᵢ` controls the sharpness of crests (Gerstner steepness). At `Qᵢ = 0` you get a plain sine wave; at `Qᵢ = 1/(wᵢ Aᵢ)` you get looping cusps.

This shader uses a **closely related but distinct** wave function in `wavedx`:

```glsl
vec2 wavedx(vec2 position, vec2 direction, float frequency, float timeshift) {
    float x = dot(direction, position) * frequency + timeshift;
    float wave = exp(sin(x) - 1.0);   // ← key difference from GPU Gems
    float dx = wave * cos(x);          // derivative of wave w.r.t. x
    return vec2(wave, -dx);
}
```

The `exp(sin(x) - 1.0)` replaces a plain `sin(x)`. Because `exp` is convex and `sin` ranges `[-1, 1]`, this maps to the range `[exp(-2), 1.0]` ≈ `[0.135, 1.0]`. The result is **peaked crests and flat troughs** — much more realistic than a symmetric sine. The derivative `dx = wave · cos(x)` is the chain rule applied to the composite: `d/dx[exp(sin(x)-1)] = exp(sin(x)-1) · cos(x)`.

The derivative matters for two reasons:
1. It's used to **drag the sample position** (simulating Gerstner's horizontal displacement).
2. It feeds into the **normal calculation** later.

### Octave Summation (`getwaves`)

A single wave frequency looks artificial. The shader sums many waves at increasing frequencies with decreasing weights — a technique called **fractional Brownian motion (fBm)** in GPU Gems and noise literature:

```glsl
float getwaves(vec2 position, int iterations) {
    float wavePhaseShift = length(position) * 0.1;
    float iter = 0.0;          // seeds the "random" direction per octave
    float frequency = 1.0;
    float timeMultiplier = 2.0;
    float weight = 1.0;
    float sumOfValues = 0.0;
    float sumOfWeights = 0.0;

    for(int i = 0; i < iterations; i++) {
        vec2 p = vec2(sin(iter), cos(iter));  // pseudo-random unit direction
        vec2 res = wavedx(position, p, frequency, iTime * timeMultiplier + wavePhaseShift);

        // Gerstner-style horizontal drag: shift sample position by wave derivative
        position += p * res.y * weight * DRAG_MULT;

        sumOfValues  += res.x * weight;
        sumOfWeights += weight;

        weight         = mix(weight, 0.0, 0.2);  // geometric decay: × 0.8 per octave
        frequency     *= 1.18;                   // frequency grows each octave
        timeMultiplier *= 1.07;
        iter += 1232.399963;                     // large irrational step → spread directions
    }
    return sumOfValues / sumOfWeights;   // normalized to [0, 1]
}
```

Key design decisions directly from GPU Gems Chapter 1:

- **Direction spread**: GPU Gems recommends distributing wave directions over a cone. Here `vec2(sin(iter), cos(iter))` with a large irrational `iter` step achieves that without explicit random number generation.
- **Frequency multiplication**: GPU Gems uses a factor between 1.1–2.0 per octave. Here it's 1.18, giving smooth detail buildup.
- **Weight decay**: GPU Gems uses `weight *= 0.82` typically. Here `mix(w, 0, 0.2)` = `w * 0.8`.
- **Position drag (`DRAG_MULT = 0.38`)**: This is the shader's analog of Gerstner's horizontal displacement `Q`. Larger values create steeper, more looping crests.

The two iteration counts (`ITERATIONS_RAYMARCH = 12`, `ITERATIONS_NORMAL = 36`) exist because normal calculation needs higher resolution detail. Raymarching can tolerate a coarser wave approximation; the final shading cannot.

---

## 3. Ray Generation

```glsl
vec3 getRay(vec2 fragCoord) {
    vec2 uv = ((fragCoord.xy / iResolution.xy) * 2.0 - 1.0)
              * vec2(iResolution.x / iResolution.y, 1.0); // aspect correction
    vec3 proj = normalize(vec3(uv.x, uv.y, 1.5));         // pinhole camera, FOV from z=1.5
    
    // apply mouse-controlled camera rotation
    return createRotationMatrixAxisAngle(vec3(0,-1,0), yawAngle)
         * createRotationMatrixAxisAngle(vec3(1, 0,0), pitchAngle)
         * proj;
}
```

This is a **pinhole camera** model. The `1.5` in `vec3(uv, 1.5)` is the focal length — decreasing it widens the field of view.

The two rotation matrices handle:
- **Yaw** (around Y): driven by mouse X, maps mouse position → horizontal pan
- **Pitch** (around X): driven by mouse Y, maps mouse position → tilt up/down

For screens narrower than 600px the mouse rotation is skipped (mobile fallback).

`createRotationMatrixAxisAngle` implements the standard **Rodrigues' rotation formula**:

```
R = cos(θ)I + sin(θ)[axis]× + (1 - cos(θ))(axis ⊗ axis)
```

You don't need to derive this; it's baked into the mat3 constructor in the shader.

---

## 4. Water Surface Intersection

The ocean surface is modeled as occupying the vertical range `[−WATER_DEPTH, 0]`. Two horizontal planes bound it:

```
waterPlaneHigh = y=0       (top of water)
waterPlaneLow  = y=−depth  (bottom, used to cap the raymarch)
```

`intersectPlane` computes the **ray-plane intersection** using the standard formula:

```
t = dot(planePoint - rayOrigin, planeNormal) / dot(rayDirection, planeNormal)
```

This gives you two world-space entry and exit points `highHitPos` and `lowHitPos` bracketing the water column along the ray. These become the raymarch start and end.

---

## 5. Raymarching the Water Surface

This is the core of the renderer. Given that waves displace the water surface between `y=0` and `y=−depth`, you need to find the exact height where the ray intersects the animated surface.

```glsl
float raymarchwater(vec3 camera, vec3 start, vec3 end, float depth) {
    vec3 pos = start;
    vec3 dir = normalize(end - start);
    for(int i = 0; i < 64; i++) {
        float height = getwaves(pos.xz, ITERATIONS_RAYMARCH) * depth - depth;
        // height is in range [-depth, 0]
        if(height + 0.01 > pos.y) {
            return distance(pos, camera);
        }
        pos += dir * (pos.y - height);  // sphere-marching style step
    }
    return distance(start, camera);    // miss: assume top plane hit
}
```

This is **sphere marching** adapted for a height field. At each step:

1. Sample the wave height at the current XZ position.
2. Compute the vertical gap `pos.y - height` (how far above the surface the ray currently is).
3. Step forward by exactly that gap — this is safe because the surface can't cross the ray in a smaller distance than the vertical gap (it's a height field, not a general SDF).

The `+ 0.01` threshold prevents infinite looping at near-grazes. The fallback at the end handles rays that don't converge (typically near the horizon where the surface is nearly parallel to the ray).

**Performance note**: 64 iterations is the inner loop. With RAYMARCH_ITERATIONS=12 for `getwaves`, that's up to 768 wave evaluations per pixel. The coarse wave count is intentional to keep this viable at real-time framerates.

---

## 6. Normal Calculation

Normals are computed by **finite differences** — sampling the height at three nearby points and taking the cross product of the two resulting tangent vectors:

```glsl
vec3 normal(vec2 pos, float e, float depth) {
    vec2 ex = vec2(e, 0);
    float H  = getwaves(pos.xy,        ITERATIONS_NORMAL) * depth;
    vec3 a   = vec3(pos.x,     H,                        pos.y);
    vec3 dX  = a - vec3(pos.x - e, getwaves(pos.xy - ex.xy, ITERATIONS_NORMAL) * depth, pos.y);
    vec3 dZ  = a - vec3(pos.x,     getwaves(pos.xy + ex.yx, ITERATIONS_NORMAL) * depth, pos.y + e);
    return normalize(cross(dX, dZ));
}
```

This mirrors the tangent/binormal normal computation from GPU Gems Ch. 1 equations (7)–(9), but done numerically instead of analytically. The analytical form from GPU Gems for a single wave is:

```
Normal = normalize(−Σ(wᵢ Aᵢ Dᵢ cos(φᵢ)), 1, −Σ(wᵢ Aᵢ Dᵢ sin(φᵢ)))
```

The numerical approach here handles the compound `exp(sin(x))` function without needing to work out its full analytical derivative across all octaves.

The `e = 0.01` step size is a tradeoff: too large → blurry normals; too small → floating-point precision issues.

After computing the normal, it's blended toward `(0, 1, 0)` (flat) with distance:

```glsl
N = mix(N, vec3(0,1,0), 0.8 * min(1.0, sqrt(dist * 0.01) * 1.1));
```

This suppresses high-frequency normal noise at long distances where sub-pixel details would alias badly — a form of level-of-detail for the normal map.

---

## 7. Fresnel Reflection

The **Fresnel effect** makes water more reflective at grazing angles and more transparent head-on. The Schlick approximation (which GPU Gems also uses) is:

```
F(θ) = F₀ + (1 − F₀)(1 − cos θ)⁵
```

where `F₀ = 0.04` is the base reflectivity of water at normal incidence (~4%). In the shader:

```glsl
float fresnel = (0.04 + (1.0 - 0.04) * pow(1.0 - max(0.0, dot(-N, ray)), 5.0));
```

`dot(-N, ray)` is the cosine of the angle between the view ray and the surface normal. Note: `ray` points *toward* the surface, so negating `N` aligns the dot product correctly.

At steep angles (looking straight down), `dot(-N, ray) ≈ 1`, so `(1 − cos)⁵ ≈ 0` and `fresnel ≈ 0.04`. At grazing angles, `cos → 0`, and `fresnel → 1.0` — almost fully reflective.

---

## 8. Atmosphere and Reflection

The reflected ray `R = reflect(ray, N)` is fed into the atmosphere function. The `abs(R.y)` ensures reflected rays always point upward (into the sky), avoiding artifacts where the reflection would look below the horizon.

### Cheap Atmosphere (`extra_cheap_atmosphere`)

This is an analytic single-scattering approximation — not physically based, but visually convincing:

```glsl
vec3 extra_cheap_atmosphere(vec3 raydir, vec3 sundir) {
    float special_trick  = 1.0 / (raydir.y * 1.0 + 0.1);   // "air mass" proxy
    float special_trick2 = 1.0 / (sundir.y * 11.0 + 1.0);  // sun horizon reddening
    float raysundt = pow(abs(dot(sundir, raydir)), 2.0);     // forward scattering (Mie-ish)
    float sundt    = pow(max(0.0, dot(sundir, raydir)), 8.0);// sun disc
    // ...
}
```

- `special_trick` is an approximation of the **optical path length** (air mass) through the atmosphere — rays near the horizon travel through more atmosphere and are thus more scattered/colored.
- `vec3(5.5, 13.0, 22.4) / 22.4` encodes the relative **Rayleigh scattering** coefficients for R, G, B — blue scatters most, giving sky its color.
- `raysundt` handles **forward scattering** (Mie-like glow around the sun).

### Moving Sun

```glsl
vec3 getSunDirection() {
    return normalize(vec3(-0.077, 0.5 + sin(iTime * 0.2 + 2.6) * 0.45, 0.577));
}
```

The Y component oscillates, simulating the sun rising and setting over ~31 seconds. The XZ components give it a fixed compass heading. At night (Y < 0), the sky goes dark.

---

## 9. Subsurface Scattering Approximation

Real ocean water scatters light internally (it looks blue/green when backlit). This shader fakes it with a constant color scaled by depth:

```glsl
vec3 scattering = vec3(0.0293, 0.0698, 0.1717) * 0.1
                * (0.2 + (waterHitPos.y + WATER_DEPTH) / WATER_DEPTH);
```

`waterHitPos.y + WATER_DEPTH` ranges from 0 (deepest point) to `WATER_DEPTH` (surface), so the scattering intensity increases near the surface where more light reaches. The color `(0.029, 0.070, 0.172)` is a deep ocean blue.

This is a **view-independent** approximation — it doesn't trace light through the volume, but it reads plausibly.

---

## 10. ACES Tonemapping

Raw HDR lighting values need to be compressed to [0, 1] for display. The shader uses the **ACES filmic curve**:

```glsl
vec3 aces_tonemap(vec3 color) {
    mat3 m1 = mat3(0.59719, 0.07600, 0.02840, ...); // input color space transform
    mat3 m2 = mat3(1.60475, -0.10208, -0.00327, ...); // output transform
    vec3 v = m1 * color;
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return pow(clamp(m2 * (a / b), 0.0, 1.0), vec3(1.0 / 2.2)); // gamma correct
}
```

`a/b` is a rational polynomial approximation of the ACES S-curve — it lifts shadows, preserves midtones, and rolls off bright highlights naturally. The final `pow(..., 1/2.2)` converts linear light to sRGB gamma.

Apply it to both sky and water, multiplied by 2.0 (global exposure boost):
```glsl
fragColor = vec4(aces_tonemap(C * 2.0), 1.0);
```

---

## 11. Putting It All Together

The full `mainImage` logic flow:

```
1. getRay(fragCoord)
       ↓
2. ray.y >= 0 ?  →  sky: getAtmosphere(ray) + getSun(ray) → tonemap → done
       ↓ (ray hits water)
3. intersectPlane × 2  →  highHitPos, lowHitPos
       ↓
4. raymarchwater(origin, highHitPos, lowHitPos, depth)  →  dist
       ↓
5. waterHitPos = origin + ray * dist
       ↓
6. normal(waterHitPos.xz, 0.01, depth)  →  N  (blended with distance)
       ↓
7. fresnel = Schlick(N, ray)
       ↓
8. R = reflect(ray, N);  R.y = abs(R.y)
   reflection = getAtmosphere(R) + getSun(R)
   scattering = subsurface color
       ↓
9. C = fresnel * reflection + scattering
       ↓
10. fragColor = aces_tonemap(C * 2.0)
```

---

## 12. Tuning Parameters

| Define | Effect | Raise | Lower |
|---|---|---|---|
| `DRAG_MULT` (0.38) | Gerstner steepness | Sharper, loopier crests | Softer, sinusoidal |
| `WATER_DEPTH` (1.0) | Layer thickness for raymarch | More parallax depth | Thinner apparent surface |
| `CAMERA_HEIGHT` (1.5) | Eye height above y=0 | More sky visible | More grazing angle |
| `ITERATIONS_RAYMARCH` (12) | Wave detail during ray-hit search | Richer surface, slower | Blockier, faster |
| `ITERATIONS_NORMAL` (36) | Wave detail for shading normals | More glint detail | Blurrier highlights |
| `DRAG_MULT × iTime` speed | Overall wave travel speed | (via `timeMultiplier`) | — |

One productive experiment: set `DRAG_MULT = 0.0` and observe that you lose Gerstner-style crest sharpening entirely — waves become pure sine sums. Then increase it back to 0.38 and watch the crests form.

---

## 13. Performance Considerations

The main cost drivers:
- `getwaves()` is called up to **64 × (raymarch iters) + 3 × (normal finite diff)** times per pixel, each with its own inner loop.
- Reduce `ITERATIONS_RAYMARCH` first for performance, then reduce the 64-step raymarch loop if needed.
- For very distant fragments (large `dist`), you could early-exit with `getwaves(..., 4)` — the low-frequency waves dominate at distance anyway.
- On desktop you can increase `ITERATIONS_NORMAL` to 64+ for sharper sun glints.

The full-screen quad has no vertex-shader bottleneck; this is purely fragment-bound.