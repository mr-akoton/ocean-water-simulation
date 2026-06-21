#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <components/Camera.hpp>
#include <components/CubeMap.hpp>
#include <core/buffers/VAO.hpp>
#include <core/buffers/VBO.hpp>
#include <core/buffers/FBO.hpp>
#include <effects/Fog.hpp>
#include <vector>

class Environment {
private:
  static const std::vector<glm::vec4> _vertices;

  int _width;
  int _height;

  VAO _vao;
  VBO _vbo;
  FBO _fbo;
  GLuint _colorTexture;
  GLuint _depthTexture;

public:
  Shader shader;

  Fog fog;
  CubeMap skybox;
  glm::vec3 skyColor;
  glm::vec3 lightDirection;
  glm::vec3 lightColor;

public:
  Environment(int width, int height);
  ~Environment();

  void render(Camera& camera);
  void attach(void) const;
  void detach(void) const;
};

#endif
