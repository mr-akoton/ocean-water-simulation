#ifndef WATER_HPP
#define WATER_HPP

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <components/Camera.hpp>
#include <components/VisualEffects.hpp>
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

  float amplitudeMult;
  float frequencyMult;
  float speedMult;
  float iterationMult;

  glm::vec3 position;
  glm::vec3 color;
  glm::mat4 model;
  Shader shader;

private:
  VAO _vao;
  VBO _vbo;
  EBO _ebo;
  std::vector<Vertex> _vertices;
  std::vector<GLuint> _indices;

public:
  Water(unsigned int width, unsigned int height, float gridSize = 1.0f,
        glm::vec3 position = glm::vec3(0.0f));
  ~Water();

  void init();
  void render(Camera& camera, const glm::vec3 lightDirection,
              const glm::vec3 lightColor, const Fog fogParam) const;
};

#endif
