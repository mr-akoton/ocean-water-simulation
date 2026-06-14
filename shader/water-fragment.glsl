#version 330 core

uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
uniform vec3 u_viewPosition;

in vec3 p_fragPosition;
in vec3 p_color;
in vec3 p_normal;

out vec4 fragColor;

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
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 512.0f) * specularStrength;

    vec3 ambient = ambientStrength * p_color;
    vec3 diffuse = diff * p_color * u_lightColor;
    vec3 specular = spec * u_lightColor;

    return ambient + diffuse + specular;
}

void main() {
    fragColor = vec4(getLight(), 1.0f);
}
