#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <imgui/imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <unordered_map>

class Window {
public:
  unsigned int width;
  unsigned int height;

private:
  GLFWwindow* _id;

  bool _isInit;
  std::unordered_map<int, bool> _previousKeys;
  std::unordered_map<int, bool> _previousButtons;

public:
  Window(void);
  ~Window();

  bool shouldClose(void) const;
  bool isKeyPressed(int key) const;
  bool isKeyJustPressed(int key);
  bool isKeyJustReleased(int key);
  bool isKeyReleased(int key) const;
  bool isButtonPressed(int button) const;
  bool isButtonJustPressed(int button);
  bool isButtonJustReleased(int button);
  bool isButtonReleased(int button) const;

  float getAxis(int negative, int positive) const;
  glm::vec2 getVector(int negativeX, int positiveX, int negativeY,
                      int positiveY) const;
  glm::vec3 getVector3(int negativeX, int positiveX, int negativeY,
                       int positiveY, int negativeZ, int positiveZ) const;

  void getCursorPosition(double& x, double& y) const;
  void setCursorPosition(double x, double y) const;
  void centerCursor(void) const;
  void hideCursor(void) const;
  void resetCursor(void) const;

  int init(unsigned int width, unsigned int height, const char* title);
  void initImGui(void) const;
  void destroy(void);
  void close(void) const;
  void update(void) const;
};

#endif
