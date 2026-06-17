#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <core/Object.hpp>
#include <core/Texture.hpp>
#include <components/CubeMap.hpp>
#include <effects/Fog.hpp>

#include <vector>

class Environment {
private:
  static const std::vector<glm::vec4> _vertices;

  VAO _vao;
  VBO _vbo;
  FBO _fbo;
  GLuint _colorTexture;
  GLuint _depthTexture;

public:
  Shader shader;

  Fog fog;
  CubeMap skybox;
  glm::vec3 lightDirection;
  glm::vec3 lightColor;

public:
  Environment(void);
  ~Environment();

  void render(Camera& camera);
  void attach(void) const;
  void detach(void) const;
};

#endif
