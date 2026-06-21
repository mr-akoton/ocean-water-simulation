#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
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
  void updateFrustumPlane(const Shader& shader) const;
  glm::mat4 getView(void) const;
  glm::mat4 getProjection(float fov, float near, float far) const;

private:
  void _handleKeyInput(const Window& window, float deltaTime);
  void _handleMouseInput(const Window& window);
};

#endif
