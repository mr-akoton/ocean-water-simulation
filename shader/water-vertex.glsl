#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

#define PI 3.14159265359

uniform mat4 u_projection;
uniform vec3 u_color;
uniform mat4 u_model;
uniform mat3 u_imodel;
uniform float u_time;

uniform int u_waveIteration;
uniform float u_amplitude;
uniform float u_frequency;
uniform float u_speed;
uniform float u_drag;
uniform float u_peakMax;
uniform float u_peakOffset;

uniform float u_amplitudeMult;
uniform float u_frequencyMult;
uniform float u_speedMult;
uniform float u_iterationMult;

out vec3 p_fragPosition;
out vec3 p_color;
out vec3 p_normal;

// Fractal wave synthesis (FBM-like sum of directional exponential waves)
//
// Base wave for each octave i:
//
//   O = dot(d_i, x) * f_i + t * s_i
//   W_i(x,t) = A_i * exp(k * sin(O) - b)
//
// where:
//   d_i : wave direction
//   A_i : amplitude
//   f_i : frequency
//   s_i : phase speed
//   k   : u_peakMax    (crest sharpness)
//   b   : u_peakOffset (vertical normalization)
//
// Total height:
//
//   H(x,t) = Sum[ W_i(x,t) ]
//
// Surface gradient:
//
//   H = Sum[ d_i * (f_i * A_i * k * exp(k * sin(O) - b) * cos(O)) ]
//
// Each octave updates amplitude, frequency, and speed to create
// a fractal (FBM) ocean surface. Position is also advected by the
// wave gradient (drag term) to introduce nonlinear wave interaction.

vec3 getWavesFBM(vec2 position) {
    float iteration = 0.0;
    float amplitude = u_amplitude;
    float frequency = u_frequency;
    float speed = u_speed;

    // Outputed value
    float height = 0.0;
    vec2 derivative = vec2(0.0);

    for (int i = 0; i < u_waveIteration; i++) {
        // Generate arbitrary direction
        vec2 direction = normalize(vec2(cos(iteration), sin(iteration)));

        float theta = dot(direction, position.xy) * frequency + u_time * speed;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        float expSine = exp(u_peakMax * sinTheta - u_peakOffset);
        float derivExpSine = u_peakMax * expSine * cosTheta;

        // 1. Accumulate wave displacement
        height += amplitude * expSine;

        // 2. Accumulate derivatives
        float WA = frequency * amplitude;
        derivative.x += direction.x * WA * derivExpSine;
        derivative.y += direction.y * WA * derivExpSine;
        position.xy -= direction * derivExpSine * amplitude * u_drag;

        // 3. Update wave parameters for next octave
        amplitude *= u_amplitudeMult;
        frequency *= u_frequencyMult;
        speed *= u_speedMult;
        iteration += u_iterationMult;
    }

    return vec3(derivative.x, height, derivative.y);
}

void main() {
    vec3 waves = getWavesFBM(in_position.xz);

    vec3 finalPosition = vec3(in_position.x, in_position.y + waves.y, in_position.z);
    vec4 worldPosition = u_model * vec4(finalPosition, 1.0);
    vec3 normal = vec3(-waves.x, 1.0, -waves.z);

    gl_Position = u_projection * worldPosition;
    p_fragPosition = worldPosition.xyz;
    p_color = u_color;
    p_normal = normalize(u_imodel * normal);
}
