#version 460 core

layout(vertices = 4) out;

uniform vec3 u_viewPosition;

uniform float u_minDivision;
uniform float u_maxDivision;
uniform float u_minDistance;
uniform float u_maxDistance;

uniform vec4 u_FPLeft;
uniform vec4 u_FPRight;
uniform vec4 u_FPBottom;
uniform vec4 u_FPTop;
uniform vec4 u_FPNear;
uniform vec4 u_FPFar;
uniform float u_heightMax;

bool AABBOutsidePlane(vec3 pmin, vec3 pmax, vec4 plane) {
    vec3 p;

    p.x = (plane.x >= 0.0) ? pmax.x : pmin.x;
    p.y = (plane.y >= 0.0) ? pmax.y : pmin.y;
    p.z = (plane.z >= 0.0) ? pmax.z : pmin.z;

    return dot(plane.xyz, p) + plane.w < 0.0;
}

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    if (gl_InvocationID == 0) {
        vec2 center = (gl_in[0].gl_Position.xy + gl_in[2].gl_Position.xy) * 0.5;

        vec3 p0 = gl_in[0].gl_Position.xzy;
        vec3 p1 = gl_in[1].gl_Position.xzy;
        vec3 p2 = gl_in[2].gl_Position.xzy;
        vec3 p3 = gl_in[3].gl_Position.xzy;

        vec3 pmin = min(min(p0, p1), min(p2, p3));
        pmin.y -= u_heightMax;
        vec3 pmax = max(max(p0, p1), max(p2, p3));
        pmax.y += u_heightMax;

        bool culled = false;
        vec4 frustumPlane[6] = {
                u_FPLeft,
                u_FPRight,
                u_FPBottom,
                u_FPTop,
                u_FPNear,
                u_FPFar
            };

        for (int i = 0; i < 6; i++) {
            if (AABBOutsidePlane(pmin, pmax, frustumPlane[i])) {
                culled = true;
                break;
            }
        }

        if (culled) {
            gl_TessLevelOuter[0] = 0.0;
            gl_TessLevelOuter[1] = 0.0;
            gl_TessLevelOuter[2] = 0.0;
            gl_TessLevelOuter[3] = 0.0;

            gl_TessLevelInner[0] = 0.0;
            gl_TessLevelInner[1] = 0.0;
        } else {
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
}
