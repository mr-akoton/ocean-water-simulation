#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <components/CubeMap.hpp>
#include <components/Environment.hpp>
#include <iostream>

static const char* VERTEX_SHADER = "shader/skybox-vertex.glsl";
static const char* FRAGMENT_SHADER = "shader/skybox-fragment.glsl";

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
    : _vao(), _vbo(this->_vertices), _ebo(this->_indices), sunSize(0.999f),
      sunBrightness(1.5f), shader(VERTEX_SHADER, FRAGMENT_SHADER) {
  _vbo.bindData(_vertices);
  _ebo.bindData(_indices);
  _vao.linkAttribute(0, 3, GL_FLOAT, 0);
  _vao.linkVBO(_vbo, 0, 0, sizeof(glm::vec3));
  _vao.linkEBO(_ebo);

  glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &texture);

  int width, height, nrChannels;

  unsigned char* firstFace =
      stbi_load(textures[0], &width, &height, &nrChannels, 0);
  if (!firstFace) {
    std::cerr << "Error: could not load texture: " << textures[0] << std::endl;
  }

  glTextureStorage2D(texture, 1, GL_RGB8, width, height);
  glTextureSubImage3D(texture, 0, 0, 0, 0, width, height, 1, GL_RGB,
                      GL_UNSIGNED_BYTE, firstFace);
  stbi_image_free(firstFace);

  for (unsigned int i = 1; i < 6; i++) {
    unsigned char* data =
        stbi_load(textures[i], &width, &height, &nrChannels, 0);
    if (data) {
      glTextureSubImage3D(texture, 0, 0, 0, i, width, height, 1, GL_RGB,
                          GL_UNSIGNED_BYTE, data);
      ;
      stbi_image_free(data);
    } else {
      std::cerr << "Error: could not load texture: " << textures[i]
                << std::endl;
    }
  }

  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  shader.enable();
  shader.setInt("u_skybox", 0);
}

CubeMap::~CubeMap() {}

/* ========================================================================== */
/*                                  RENDERER                                  */
/* ========================================================================== */

void CubeMap::render(Camera& camera, Environment& environment) {
  glDepthFunc(GL_LEQUAL);
  glDisable(GL_CULL_FACE);

  _vao.bind();
  shader.enable();

  glm::mat4 view(1.0f);
  glm::mat4 projection(1.0f);
  view = glm::mat4(glm::mat3(glm::lookAt(
      camera.position, camera.position + camera.orientation, camera.up)));
  projection = glm::perspective(
      glm::radians(45.0f), (float)camera.width / camera.height, 0.1f, 100.0f);

  shader.setMat4("u_view", view);
  shader.setMat4("u_projection", projection);

  shader.setVec3("u_viewPosition", camera.position);
  shader.setVec3("u_skyColor", environment.skyColor);
  shader.setVec3("u_lightDirection", environment.lightDirection);
  shader.setVec3("u_lightColor", environment.lightColor);

  shader.setFloat("u_sunSize", sunSize);
  shader.setFloat("u_sunBrightness", sunBrightness);

  glBindTextureUnit(0, texture);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  _vao.unbind();

  glEnable(GL_CULL_FACE);
  glDepthFunc(GL_LESS);
}
