#version 330 core

uniform samplerCube u_skybox;

in vec3 p_textureCoords;

out vec4 FragColor;

void main() {
    FragColor = texture(u_skybox, p_textureCoords);
}
