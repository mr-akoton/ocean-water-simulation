#version 460 core

#define PI 3.14159265359

uniform samplerCube u_skybox;
uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
uniform vec3 u_viewPosition;
uniform float u_heightMax;

uniform vec3 u_emissivity;
uniform vec3 u_baseReflectance;
uniform float u_roughness;
uniform float u_metallic;

uniform vec3 u_scatterColor;
uniform float u_scatterStrength;
uniform float u_scatterPower;
uniform float u_scatterDistortion;

uniform vec3 u_foamColor;
uniform float u_foamHeight;
uniform float u_foamSteepness;
uniform float u_foamJacobian;

in vec3 p_fragPosition;
in vec3 p_color;
in vec3 p_normal;
in float p_jacobian;
in vec2 p_derivative;

out vec4 FragColor;

vec3 getLight(vec3 color) {
    // Basic vector for calculation
    vec3 N = normalize(p_normal); // Normal vector
    vec3 L = normalize(-u_lightDirection); // Light vector
    vec3 V = normalize(u_viewPosition - p_fragPosition); // View vector
    vec3 H = normalize(L + V); // Half-way vector
    vec3 R = reflect(-V, N); // Reflection of the view vector

    // Precomputed value
    float VH = max(dot(V, H), 0.0);
    float HN = max(dot(H, N), 0.0);
    float NL = max(dot(N, L), 0.0);
    float NV = max(dot(N, V), 0.0);

    // BRDF: Bidirectional Reflectance Distribution Function
    // BRDF = k_d * f_diffuse + k_s * f_specular where k_d + k_s = 1.0 and k_s = fresnel
    // Fernsel = F0 + (1.0 - F0) * (1.0 - (V . H))^5
    // Note: Using NV instead of VH and got a better result for some reason (Will find why later)
    vec3 F0 = mix(u_baseReflectance, color, u_metallic);
    vec3 k_s = F0 + (vec3(1.0) - F0) * pow(1.0 - NV, 5.0);
    vec3 k_d = (vec3(1.0) - k_s) * (1.0 - u_metallic);

    // Lambertian Diffuse: diffuse = (color / PI) * (L . N) where L . N will be discard for later calculation
    vec3 diffuse = color / PI;

    // Cook Torrance Specular: specular = D * G * F / (4 * (V . N) * (L . N))
    // F: Fresnel Function (a.k.a k_d)

    // D: Normal Distribution Function
    // Used D_GXX / Trowbirdge - Reitz Model
    // D = r^2 / PI * ( (H . N)^2 (r^2 * - 1) + 1 )^2
    float roughtness2 = u_roughness * u_roughness;
    float a = pow(HN, 2.0) * (roughtness2 - 1.0) + 1.0;
    float normalDist = roughtness2 / max(PI * (a * a), 0.000001);

    // G: Geometry Shadowing Function
    // Used Schilck-GGX model
    // G_X = (N . X) / ((N . X) * (1 - k) + k)
    float k = u_roughness / 2.0;
    // float k = (u_roughness + 1.0) * (u_roughness + 1.0) / 8.0;
    float geoV = NV / max(NV * (1.0 - k) + k, 0.000001);
    float geoL = NL / max(NL * (1.0 - k) + k, 0.000001);
    float geometryShad = geoV * geoL;

    vec3 specular = (normalDist * geometryShad * k_s) / max(4.0 * NV * NL, 0.000001);

    // BRDF
    vec3 BRDF = k_d * diffuse + specular;
    vec3 light = u_emissivity + BRDF * u_lightColor * NL;

    // Env Reflection
    float mip = u_roughness * 8.0;
    vec3 env = textureLod(u_skybox, R, mip).rgb;
    vec3 reflection = env * k_s;

    // Scattered Light
    float waveHeight = clamp(p_fragPosition.y / u_heightMax, 0.0, 1.0);
    vec3 LT = normalize(L + N * u_scatterDistortion);

    float viewScatter = pow(max(dot(V, -LT), 0.0), u_scatterPower);
    float backlit = pow(max(0.0, 0.5 - 0.5 * max(dot(L, N), -0.2)), 2.0);

    vec3 scatter = waveHeight * viewScatter * backlit * u_scatterStrength * u_scatterColor * u_lightColor;

    // Sea foam
    float foamFromJacobian = 1.0 - smoothstep(0.0, 1.0, p_jacobian);
    float foamFromSteepness = length(p_derivative);
    float foamCrest = (u_foamSteepness * foamFromSteepness + u_foamJacobian * foamFromJacobian) * smoothstep(u_foamHeight, 1.0, p_fragPosition.y / u_heightMax);
    vec3 foam = foamCrest * u_foamColor;

    return light + reflection + scatter + foam;
}

void main() {
    vec3 color = getLight(p_color);
    FragColor = vec4(color, 1.0);
}
