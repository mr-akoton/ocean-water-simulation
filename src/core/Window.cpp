#include <core/Window.hpp>

using namespace glm;

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Window::Window(void) : width(0), height(0), _id(nullptr), _isInit(false) {}

Window::~Window() { destroy(); }

/* ========================================================================== */
/*                              GETTER AND SETTER                             */
/* ========================================================================== */

bool Window::shouldClose(void) const {
  return _isInit and glfwWindowShouldClose(_id);
}

bool Window::isKeyPressed(int key) const {
  return _isInit and glfwGetKey(_id, key) == GLFW_PRESS;
}

bool Window::isKeyJustPressed(int key) {
  if (not _isInit) {
    return false;
  }

  bool current = glfwGetKey(_id, key) == GLFW_PRESS;
  bool previous = _previousKeys[key];
  _previousKeys[key] = current;
  return current and not previous;
}

bool Window::isKeyJustReleased(int key) {
  if (not _isInit) {
    return false;
  }

  bool current = glfwGetKey(_id, key) == GLFW_PRESS;
  bool previous = _previousKeys[key];
  _previousKeys[key] = current;
  return not current and previous;
}

bool Window::isKeyReleased(int key) const {
  return _isInit and glfwGetKey(_id, key) == GLFW_RELEASE;
}

bool Window::isButtonPressed(int button) const {
  return _isInit and glfwGetMouseButton(_id, button) == GLFW_PRESS;
}

bool Window::isButtonJustPressed(int button) {
  if (not _isInit) {
    return false;
  }

  bool current = glfwGetMouseButton(_id, button) == GLFW_PRESS;
  bool previous = _previousButtons[button];
  _previousButtons[button] = current;
  return current and not previous;
}

bool Window::isButtonJustReleased(int button) {
  if (not _isInit) {
    return false;
  }

  bool current = glfwGetMouseButton(_id, button) == GLFW_PRESS;
  bool previous = _previousButtons[button];
  _previousButtons[button] = current;
  return not current and previous;
}

bool Window::isButtonReleased(int button) const {
  return _isInit and glfwGetMouseButton(_id, button) == GLFW_RELEASE;
}

float Window::getAxis(int negative, int positive) const {
  return isKeyPressed(positive) - isKeyPressed(negative);
}

vec2 Window::getVector(int negativeX, int positiveX, int negativeY,
                       int positiveY) const {
  const vec2 output(getAxis(negativeX, positiveX),
                    getAxis(negativeY, positiveY));
  return length(output) == 0.0f ? output : normalize(output);
}

vec3 Window::getVector3(int negativeX, int positiveX, int negativeY,
                        int positiveY, int negativeZ, int positiveZ) const {
  const vec3 output(getAxis(negativeX, positiveX),
                    getAxis(negativeY, positiveY),
                    getAxis(negativeZ, positiveZ));
  return length(output) == 0.0f ? output : normalize(output);
}

void Window::getCursorPosition(double& x, double& y) const {
  glfwGetCursorPos(_id, &x, &y);
}

void Window::setCursorPosition(double x, double y) const {
  glfwSetCursorPos(_id, x, y);
}

void Window::centerCursor(void) const {
  glfwSetCursorPos(_id, (double)width / 2.0, (double)height / 2.0);
}

void Window::hideCursor(void) const {
  glfwSetInputMode(_id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::resetCursor(void) const {
  glfwSetInputMode(_id, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

/* ========================================================================== */
/*                                    INIT                                    */
/* ========================================================================== */

static void frameBufferSizeCallback(GLFWwindow*, int width, int height) {
  glViewport(0, 0, width, height);
}

int Window::init(unsigned int width, unsigned int height, const char* title) {
  if (_isInit or width < 100 or height < 100 or title == nullptr) {
    return -1;
  }

  this->width = width;
  this->height = height;

  _id = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (_id == nullptr) {
    return -1;
  }

  _isInit = true;
  glfwMakeContextCurrent(_id);
  glfwSetFramebufferSizeCallback(_id, frameBufferSizeCallback);
  centerCursor();

  return 0;
}

void Window::initImGui(void) const { ImGui_ImplGlfw_InitForOpenGL(_id, true); }

/* ========================================================================== */
/*                                   UPDATE                                   */
/* ========================================================================== */

void Window::update(void) const {
  if (not _isInit) {
    return;
  }

  glfwSwapBuffers(_id);
}

/* ========================================================================== */
/*                                  TERMINATE                                 */
/* ========================================================================== */

void Window::close(void) const {
  if (not _isInit) {
    return;
  }

  if (not this->shouldClose()) {
    glfwSetWindowShouldClose(_id, true);
  }
}

void Window::destroy(void) {
  if (not _isInit) {
    return;
  }

  glfwDestroyWindow(_id);
  _id = nullptr;
}
