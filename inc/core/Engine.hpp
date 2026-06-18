#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "components/Environment.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <components/Camera.hpp>
#include <components/Interface.hpp>
#include <components/Water.hpp>
#include <core/Object.hpp>
#include <core/Shader.hpp>
#include <core/Window.hpp>
#include <effects/Fog.hpp>
#include <settings/SettingsManager.hpp>

#include <exception>

class Engine {
public:
  Window window;
  Camera camera;
  Interface interface;

private:
  bool _isUIEnable;
  float _lastFrame;
  float _deltaTime;
  unsigned int _frameCount;

public:
  Engine(void);
  ~Engine();

  void run(void);

private:
  void _initGLFW(void) const;
  void _initGLAD(void) const;
  void _handleInput(void);
  void _updateDeltaTime(void);
  void _displayUI(Water& water, Environment& environment, char* fpsText);
  void _loadSettings(Water& water, Environment& environment);
  void _saveSettings(const Water& water, const Environment& environment);

public:
  class WindowInitFailedException : std::exception {
    virtual const char* what(void) const noexcept;
  };

  class GladLoadFailedException : std::exception {
    virtual const char* what(void) const noexcept;
  };
};

#endif
