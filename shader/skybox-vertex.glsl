#version 460 core

layout(location = 0) in vec3 in_position;

uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 p_textureCoords;

void main() {
    vec4 position = u_projection * u_view * vec4(in_position, 1.0f);
    gl_Position = vec4(position.x, position.y, position.w, position.w);
    p_textureCoords = vec3(in_position.x, in_position.y, -in_position.z);
}
