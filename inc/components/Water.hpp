#ifndef WATER_HPP
#define WATER_HPP

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <components/Camera.hpp>
#include <components/Environment.hpp>
#include <core/Object.hpp>
#include <core/Shader.hpp>

#include <vector>

class Water {
public:
  unsigned int width;
  unsigned int height;
  float gridSize;

  int iteration;
  float amplitude;
  float frequency;
  float speed;
  float drag;
  float peakMax;
  float peakOffset;
  std::vector<glm::vec2> waveDirections;

  float amplitudeMult;
  float frequencyMult;
  float speedMult;
  float iterationMult;

  glm::vec3 ambientColor;
  float ambientStrength;
  float specularStrength;
  int shininess;

  glm::vec3 position;
  glm::vec3 color;
  glm::mat4 model;
  glm::mat4 imodel;
  Shader shader;

private:
  VAO _vao;
  VBO _vbo;
  EBO _ebo;
  GLuint _directionTexture;
  std::vector<glm::vec2> _vertices;
  std::vector<GLuint> _indices;
  GLuint _indicesCount;

public:
  Water(unsigned int width, unsigned int height, float gridSize = 1.0f);
  ~Water();

  void init();
  void render(Camera& camera, Environment& environment) const;
};

#endif
