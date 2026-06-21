#version 460 core

#define PI 3.14159265359

uniform samplerCube u_skybox;
uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
uniform vec3 u_viewPosition;

// uniform float u_ambientStrength;
// uniform vec3 u_ambientColor;
// uniform float u_specularStrength;
// uniform int u_shininess;

uniform vec3 u_emissivity;
uniform vec3 u_baseReflectance;
uniform float u_roughness;
uniform float u_metallic;

in vec3 p_fragPosition;
in vec3 p_color;
in vec3 p_normal;

out vec4 FragColor;

// vec3 getLight() {
//     vec3 normal = normalize(p_normal);

//     vec3 lightDirection = normalize(-u_lightDirection);
//     vec3 viewDirection = normalize(u_viewPosition - p_fragPosition);

//     // Fresnel (Schlick approximation)
//     float fresnel = pow(1.0 - max(dot(normal, viewDirection), 0.0), 5.0);

//     // Reflection for environment mapping
//     vec3 refl = reflect(-viewDirection, normal);

//     // Diffuse light
//     float diff = max(dot(normal, lightDirection), 0.0);

//     // Specular reflection
//     vec3 halfwayDir = normalize(lightDirection + viewDirection);
//     float spec = pow(max(dot(normal, halfwayDir), 0.0), u_shininess) * u_specularStrength * fresnel;

//     vec3 ambient = u_ambientStrength * u_ambientColor * p_color;
//     vec3 diffuse = diff * p_color * u_lightColor;
//     vec3 specular = spec * u_lightColor;
//     vec3 reflection = texture(u_skybox, refl).rgb * fresnel;

//     return ambient + diffuse + specular + reflection;
// }

vec3 getLight(vec3 color) {
    // Basic vector for calculation
    vec3 N = normalize(p_normal); // Normal vector
    vec3 L = normalize(-u_lightDirection); // Light vector
    vec3 V = normalize(u_viewPosition - p_fragPosition); // View vector
    vec3 H = normalize(L + V); // Half-way vector
    vec3 R = reflect(-V, N);

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

    float mip = u_roughness * 8.0;
    vec3 env = textureLod(u_skybox, R, mip).rgb;
    vec3 reflection = env * k_s;

    return light + reflection;
}

void main() {
    vec3 color = getLight(p_color);
    FragColor = vec4(color, 1.0);
}
