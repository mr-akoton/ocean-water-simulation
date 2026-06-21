#version 460 core

uniform vec3 u_viewPosition;

uniform mat4 u_invProjection;
uniform mat4 u_invView;
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

float fogTop = 1000.0f;
float fogBottom = 0.0f;

float linearizeDepth(float depth) {
    return (2.0 * u_fogNear * u_fogFar) / (u_fogFar + u_fogNear - (depth * 2.0 - 1.0) * (u_fogFar - u_fogNear));
}

float logisticDepth(float depth) {
    float zValue = linearizeDepth(depth);
    return (1.0 / (1.0 + exp(-u_fogSteepness * (zValue - u_fogOffset))));
}

vec3 getWorldPosition(vec2 uv, float depth) {
    // UV -> Clip space
    vec2 ndc;
    ndc = uv * 2.0 - 1.0;
    vec4 clipPosition = vec4(ndc, depth * 2.0 - 1.0, 1.0);

    // View space
    vec4 viewPosition = u_invProjection * clipPosition;
    viewPosition /= viewPosition.w;

    // World space
    vec4 worldPosition = u_invView * viewPosition;

    return worldPosition.xyz;
}

void main() {
    float depth = texture(u_depthTexture, p_textureCoords).r;
    float distanceFog = logisticDepth(depth);
    vec3 finalColor = texture(u_colorTexture, p_textureCoords).rgb;

    vec3 worldPosition = getWorldPosition(p_textureCoords, depth);
    float heightFog = 1.0 - smoothstep(fogBottom, fogTop, worldPosition.y);

    float fogFactor = distanceFog * heightFog;

    if (u_enableFog) {
        finalColor = mix(finalColor, u_fogColor, fogFactor);
    }

    // Tone mapping and gamma correction
    // finalColor = finalColor / (finalColor + vec3(1.0));
    // finalColor = pow(finalColor, vec3(1.0 / 2.2));

    FragColor = vec4(finalColor, 1.0);
}
