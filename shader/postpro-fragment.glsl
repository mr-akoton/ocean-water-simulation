#version 330 core

uniform sampler2D u_screenTexture;

in vec2 p_textureCoords;

out vec4 FragColor;

void main() {
    FragColor = texture(u_screenTexture, p_textureCoords);
}
