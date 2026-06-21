#ifndef WATER_HPP
#define WATER_HPP

#include "glm/ext/vector_float3.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <components/Camera.hpp>
#include <components/Environment.hpp>
#include <core/buffers/VAO.hpp>
#include <core/buffers/VBO.hpp>
#include <core/buffers/EBO.hpp>
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

  glm::vec3 emissivity;
  glm::vec3 baseReflectance;
  float roughness;
  float metallic;

  glm::vec3 scatterColor;
  float scatterStrength;
  float scatterPower;
  float scatterDistortion;

  float minDivision;
  float maxDivision;
  float minDistance;
  float maxDistance;

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
