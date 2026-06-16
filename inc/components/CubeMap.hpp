#ifndef CUBE_MAP_HPP
#define CUBE_MAP_HPP

#include <glm/glm.hpp>
#include <stb/stb_image.h>

#include <components/Camera.hpp>
#include <core/Shader.hpp>
#include <core/Object.hpp>

#include <vector>

class CubeMap {
private:
  static const std::vector<glm::vec3> _vertices;
  static const std::vector<GLuint> _indices;

  VAO _vao;
  VBO _vbo;
  EBO _ebo;

public:
  float sunSize;
  float sunBrightness;
  GLuint texture;
  Shader shader;

public:
  CubeMap(const char* textures[6]);
  ~CubeMap();

  void render(Camera& camera, const glm::vec3 lightDirection,
              const glm::vec3 lightColor, const glm::vec3 skyColor);
};

#endif
