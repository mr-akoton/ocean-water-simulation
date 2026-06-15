#version 330 core

uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
uniform vec3 u_viewPosition;

uniform float u_fogNear;
uniform float u_fogFar;
uniform float u_fogSteepness;
uniform float u_fogOffset;
uniform vec3 u_fogColor;

in vec3 p_fragPosition;
in vec3 p_color;
in vec3 p_normal;

out vec4 FragColor;

// float linearizeDepth(float depth) {
//     return (2.0 * u_fogNear * u_fogFar) / (u_fogFar + u_fogNear - (depth * 2.0 - 1.0) * (u_fogFar - u_fogNear));
// }

// float logisticDepth(float depth) {
//     float zValue = linearizeDepth(depth);
//     return (1.0 / (1.0 + exp(-u_fogSteepness * (zValue - u_fogOffset))));
// }

vec3 getLight() {
    float ambientStrength = 0.2;
    float specularStrength = 0.4;

    // Diffuse lighting
    vec3 normal = normalize(p_normal);
    vec3 lightDirection = normalize(-u_lightDirection);
    float diff = max(dot(lightDirection, normal), 0.0);

    // Specular reflection
    vec3 viewDirection = normalize(u_viewPosition - p_fragPosition);
    vec3 halfwayDir = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 512.0) * specularStrength;

    vec3 ambient = ambientStrength * p_color;
    vec3 diffuse = diff * p_color * u_lightColor;
    vec3 specular = spec * u_lightColor;

    return ambient + diffuse + specular;
}

void main() {
    // float depth = logisticDepth(gl_FragCoord.z);
    // FragColor = vec4(getLight(), 1.0) * (1.0 - depth) + vec4(depth * u_fogColor, 1.0);
    FragColor = vec4(getLight(), 1.0);
}
