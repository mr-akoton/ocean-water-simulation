# Water Simulation

A procedural water simulation and renderer written in **C++17** using **OpenGL 3.3**.  
Built to explore and understand sinewave-based ocean surfaces, skybox rendering, and post-processing effects in modern OpenGL.

---

## 🧩 Features

### 🌊 Water Simulation
- **FBM wave synthesis** — Multi-octave fractal sum of directional exponential waves for realistic ocean surfaces.
- **Exponential wave shaping** — Controllable crest sharpness via `exp(k * sin(theta) - b)`.
- **Choppy waves** — Horizontal vertex displacement toward wave crests using a drag term.
- **Normal mapping from derivatives** — Surface normals derived analytically from the wave function for accurate lighting.
- **Blinn-Phong lighting** — Ambient, diffuse, and specular components with fully adjustable material properties.

### 🌅 Skybox & Sun
- **Cubemap skybox** — Rendered from 6 texture faces, always centered on the camera.
- **Procedural sun** — Hard disc with a soft glow halo, positioned dynamically based on the light direction.
- **Adjustable sun size and brightness** via the UI.

### 🎨 Post-Processing
- **FBO pipeline** — Scene rendered to an off-screen framebuffer, then composited with effects.
- **Distance fog** — Logistic curve-based depth fog with adjustable steepness and offset.
- **Height fog** — World-space Y-based fog blending using `smoothstep`.
- **Combined fog** — Distance and height fog multiplied together for natural, seamless transitions.

### 🖥️ User Interface
- **Dear ImGui** (docking branch) — Full runtime parameter control without recompiling.
- **Toggle UI** with the `T` key.
- Adjustable parameters:
  - Water color, amplitude, frequency, speed, iterations, drag, wave shaping, and multipliers
  - Material properties: ambient, specular, shininess
  - Sky and light color, light direction
  - Sun size and brightness
  - All fog settings (steepness, offset, height blend)

### 📷 Camera System
- **Free-fly 3D camera** — WASD movement, Space/Shift for vertical axis, mouse drag for rotation.
- **Pitch clamping** to prevent gimbal lock.
- **Frame-rate independent movement** via delta time.

---

## 🚀 Getting Started

### Prerequisites

This project is developed and tested on **Linux only**. Building on other operating systems is not guaranteed.

You will need:
- A **C++17** compatible compiler (e.g. `g++` or `clang++`)
- An **OpenGL 3.3** compatible GPU and drivers
- **GLFW3** installed system-wide (`libglfw3-dev` on Debian/Ubuntu)
- **Make** build tool

Install dependencies on Debian/Ubuntu:
```bash
sudo apt install build-essential libglfw3-dev
```

Install dependencies on Arch:
```bash
sudo pacman -S base-devel glfw
```

### Compile and Run

```bash
git clone git@github.com:mr-akoton/water-simulation.git
cd water-simulation && make
./water-simulation
```

---

## 🎮 Controls

| Input | Action |
|---|---|
| `W / A / S / D` | Move forward / left / backward / right |
| `Space` | Move up |
| `Shift` | Move down |
| `Left Mouse Button` + drag | Look around |
| `T` | Toggle settings UI |
| `Escape` | Quit |

---

## 🔗 References & Documentation

- [**Victor Gordan** — OpenGL Tutorial Series](https://www.youtube.com/watch?v=XpBGwZNyUh0&list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-)
- [**Acerola** — How Games Fake Water](https://youtu.be/PH9q0HNBjT4?si=kZ0-5nDRtUmomY7x)
- [**GPU Gems** — Effective Water Simulation from Physical Models](https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models)
- [**Wikipedia** — Blinn-Phong Reflection Model](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model)
