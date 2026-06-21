#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <components/Camera.hpp>
#include <components/Environment.hpp>
#include <components/Interface.hpp>
#include <components/Water.hpp>
#include <core/Window.hpp>
#include <settings/SettingsPanel.hpp>
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
  SettingsPanel _settingsPanel;

public:
  Engine(void);
  ~Engine();

  void run(void);

private:
  void _initGLFW(void) const;
  void _initGLAD(void) const;
  void _handleInput(void);
  void _updateDeltaTime(void);
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
