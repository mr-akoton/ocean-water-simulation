#version 330 core

uniform vec3 u_viewPosition;
uniform sampler2D u_colorTexture;
uniform sampler2D u_depthTexture;

uniform bool u_enableFog;

uniform float u_fogNear;
uniform float u_fogFar;
uniform float u_fogSteepness;
uniform float u_fogOffset;
uniform vec3 u_fogColor;

in vec2 p_textureCoords;

out vec4 FragColor;

float linearizeDepth(float depth) {
    return (2.0 * u_fogNear * u_fogFar) / (u_fogFar + u_fogNear - (depth * 2.0 - 1.0) * (u_fogFar - u_fogNear));
}

float logisticDepth(float depth) {
    float zValue = linearizeDepth(depth);
    return (1.0 / (1.0 + exp(-u_fogSteepness * (zValue - u_fogOffset))));
}

void main() {
    float depth = logisticDepth(texture(u_depthTexture, p_textureCoords).r);
    vec3 final_color = texture(u_colorTexture, p_textureCoords).rgb;

    if (u_enableFog) {
        final_color = mix(final_color, u_fogColor, depth);
    }

    FragColor = vec4(final_color, 1.0);
}
