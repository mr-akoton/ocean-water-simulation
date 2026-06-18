#ifndef CAMERA_HPP
#define CAMERA_HPP

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <core/Shader.hpp>
#include <core/Window.hpp>

class Camera {
public:
  bool isFirstClick;

  unsigned int width;
  unsigned int height;
  float ratio;
  float speed;
  float sensitivity;

  glm::vec3 up;
  glm::vec3 position;
  glm::vec3 orientation;
  glm::mat4 matrix;

public:
  Camera(unsigned int width, unsigned int height, glm::vec3 position);

  void handleInput(const Window& window, float deltaTime);
  void updateMatrix(float fov, float near, float far);
  void updateShaderMatrix(const Shader& shader,
                          const std::string uniform) const;
  glm::mat4 getView(void) const;
  glm::mat4 getProjection(float fov, float near, float far) const;

private:
  void _handleKeyInput(const Window& window, float deltaTime);
  void _handleMouseInput(const Window& window);
};

#endif
