#version 330 core

uniform vec3 u_viewPosition;
uniform sampler2D u_colorTexture;
uniform sampler2D u_depthTexture;

uniform float u_near;
uniform float u_far;
uniform float u_steepness;
uniform float u_offset;
uniform vec3 u_fogColor;

in vec2 p_textureCoords;

out vec4 FragColor;

float linearizeDepth(float depth) {
    return (2.0 * u_near * u_far) / (u_far + u_near - (depth * 2.0 - 1.0) * (u_far - u_near));
}

float logisticDepth(float depth) {
    float zValue = linearizeDepth(depth);
    return (1.0 / (1.0 + exp(-u_steepness * (zValue - u_offset))));
}

void main() {
    float depth = logisticDepth(texture(u_depthTexture, p_textureCoords).r);
    vec3 color = texture(u_colorTexture, p_textureCoords).rgb;

    FragColor = vec4(mix(color, u_fogColor, depth), 1.0);
}
