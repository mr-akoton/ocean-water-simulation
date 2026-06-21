#version 460 core

uniform samplerCube u_skybox;
uniform vec3 u_skyColor;
uniform vec3 u_viewPosition;
uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
uniform float u_sunSize;
uniform float u_sunBrightness;

in vec3 p_textureCoords;

out vec4 FragColor;

void main() {
    // Sun
    vec3 direction = normalize(vec3(-p_textureCoords.x, -p_textureCoords.y, p_textureCoords.z));
    float sunDot = dot(direction, normalize(u_lightDirection));

    // Hard sund disk
    float sunDisk = smoothstep(u_sunSize - 0.0005, u_sunSize + 0.0005, sunDot);
    float sunGlow = pow(max(sunDot, 0.0), 32.0);
    vec3 sunColor = u_lightColor * u_sunBrightness;

    // Composite
    vec3 finalColor = texture(u_skybox, p_textureCoords).rgb + sunColor * (sunDisk + sunGlow * 0.3);

    FragColor = vec4(finalColor, 1.0);
}
