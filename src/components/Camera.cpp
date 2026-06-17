#include <components/Camera.hpp>

using namespace glm;

/* ========================================================================== */
/*                                 CONSTRUCTOR                                */
/* ========================================================================== */

Camera::Camera(unsigned int width, unsigned int height, vec3 position)
    : isFirstClick(true), width(width), height(height),
      ratio((float)width / height), speed(100.0f), sensitivity(100.0f),
      up(0.0f, 1.0f, 0.0f), position(position), orientation(0.0f, 0.0f, 1.0f),
      matrix(1.0f) {}

/* ========================================================================== */
/*                                    INPUT                                   */
/* ========================================================================== */

void Camera::handleInput(const Window& window, float deltaTime) {
  _handleKeyInput(window, deltaTime);
  _handleMouseInput(window);
}

void Camera::_handleKeyInput(const Window& window, float deltaTime) {
  const float cameraSpeed = speed * deltaTime;

  if (window.isKeyPressed(GLFW_KEY_W)) {
    position += orientation * cameraSpeed;
  } else if (window.isKeyPressed(GLFW_KEY_S)) {
    position -= orientation * cameraSpeed;
  }

  if (window.isKeyPressed(GLFW_KEY_D)) {
    position += normalize(cross(orientation, up)) * cameraSpeed;
  } else if (window.isKeyPressed(GLFW_KEY_A)) {
    position -= normalize(cross(orientation, up)) * cameraSpeed;
  }

  if (window.isKeyPressed(GLFW_KEY_SPACE)) {
    position += up * cameraSpeed;
  } else if (window.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
    position -= up * cameraSpeed;
  }
}

void Camera::_handleMouseInput(const Window& window) {
  if (window.isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
    window.hideCursor();

    if (isFirstClick) {
      window.centerCursor();
      isFirstClick = false;
    }

    float rotx, roty;
    double cursorx, cursory;
    vec3 newOrientation;

    window.getCursorPosition(cursorx, cursory);
    rotx = sensitivity * (float)(cursory - (height / 2.0f)) / height;
    roty = sensitivity * (float)(cursorx - (width / 2.0f)) / width;

    newOrientation =
        rotate(orientation, radians(-rotx), normalize(cross(orientation, up)));

    if (abs(angle(newOrientation, up) - radians(90.0f)) <= radians(85.0f)) {
      orientation = newOrientation;
    }

    orientation = rotate(orientation, radians(-roty), up);
    window.centerCursor();
  } else if (window.isButtonReleased(GLFW_MOUSE_BUTTON_1)) {
    window.resetCursor();
    isFirstClick = true;
  }
}

/* ========================================================================== */
/*                                   GETTER                                   */
/* ========================================================================== */

mat4 Camera::getView(void) const {
  return lookAt(position, position + orientation, up);
}

mat4 Camera::getProjection(float fov, float near, float far) const {
  return perspective(radians(fov), ratio, near, far);
}

/* ========================================================================== */
/*                                   UPDATE                                   */
/* ========================================================================== */

void Camera::updateMatrix(float fov, float near, float far) {
  mat4 view(1.0f);
  mat4 projection(1.0f);

  view = lookAt(position, position + orientation, up);
  projection = perspective(radians(fov), ratio, near, far);
  matrix = projection * view;
}

void Camera::updateShaderMatrix(const Shader& shader,
                                const std::string uniform) const {
  shader.setMat4(uniform, matrix);
}
