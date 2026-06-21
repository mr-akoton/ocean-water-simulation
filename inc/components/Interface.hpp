#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <imgui/imgui.h>
#include <core/Window.hpp>

class Interface {
public:
  Window* window;
  ImGuiIO* io;

private:
  bool _isInit;

public:
  Interface(void);
  ~Interface();

  void init(Window& window);
  void render(void) const;
  void shutdown(void);

  bool wantCaptureMouse(void) const;
  void createFrame(void) const;
};

#endif
