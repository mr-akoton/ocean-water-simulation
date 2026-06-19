#version 460 core

layout(vertices = 4) out;

uniform vec3 u_viewPosition;

uniform float u_minDivision;
uniform float u_maxDivision;
uniform float u_minDistance;
uniform float u_maxDistance;

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0) {
        vec2 center = (gl_in[0].gl_Position.xy + gl_in[2].gl_Position.xy) * 0.5;

        float distance = length(u_viewPosition.xz - center);
        float t = clamp((distance - u_maxDistance) / (u_maxDistance - u_minDistance), 0.0, 1.0);
        float tess = mix(u_maxDivision, u_minDivision, t);

        gl_TessLevelOuter[0] = tess;
        gl_TessLevelOuter[1] = tess;
        gl_TessLevelOuter[2] = tess;
        gl_TessLevelOuter[3] = tess;

        gl_TessLevelInner[0] = tess;
        gl_TessLevelInner[1] = tess;
    }
}
