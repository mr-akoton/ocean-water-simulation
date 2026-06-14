#include "components/Camera.hpp"
#include "core/Engine.hpp"
#include "core/Object.hpp"
#include "core/Shader.hpp"
#include "core/Texture.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include <components/CubeMap.hpp>
#include <iostream>
#include <ostream>
#include <vector>

const std::vector<glm::vec3> CubeMap::_vertices{
    {-1.0f, -1.0f, +1.0f}, {+1.0f, -1.0f, +1.0f}, {+1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f}, {-1.0f, +1.0f, +1.0f}, {+1.0f, +1.0f, +1.0f},
    {+1.0f, +1.0f, -1.0f}, {-1.0f, +1.0f, -1.0f},
};
const std::vector<GLuint> CubeMap::_indices{
    1, 2, 6, 6, 5, 1, // Right
    0, 4, 7, 7, 3, 0, // Left
    4, 5, 6, 6, 7, 4, // Top
    0, 3, 2, 2, 1, 0, // Bottom
    0, 1, 5, 5, 4, 0, // Back
    3, 7, 6, 6, 2, 3, // Front
};

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

CubeMap::CubeMap(const char* textures[6])
    : _vao(), _vbo(this->_vertices), _ebo(this->_indices),
      shader("shader/skybox-vertex.glsl", "shader/skybox-fragment.glsl") {
  _vao.bind();
  _vao.linkAttribute(_vbo, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
  _ebo.bind();

  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  for (unsigned int i = 0; i < 6; i++) {
    int width, height, nrChannels;

    if (i == 0) {
      stbi_set_flip_vertically_on_load(true);
    } else {
      stbi_set_flip_vertically_on_load(false);
    }

    unsigned char* data =
        stbi_load(textures[i], &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cerr << "Error: could not load texture: " << textures[i]
                << std::endl;
      stbi_image_free(data);
    }
  }
}

CubeMap::~CubeMap() {}

void CubeMap::render(Camera& camera) {
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);

  _vao.bind();
  shader.enable();

  glm::mat4 view(1.0f);
  glm::mat4 projection(1.0f);
  view = glm::mat4(glm::mat3(glm::lookAt(
      camera.position, camera.position + camera.orientation, camera.up)));
  projection = glm::perspective(
      glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

  shader.setMat4("u_view", view);
  shader.setMat4("u_projection", projection);
  shader.setInt("u_skybox", 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  _vao.unbind();

  glEnable(GL_CULL_FACE);
  glDepthFunc(GL_LESS);
};
