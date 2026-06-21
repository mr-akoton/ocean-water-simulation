#include <cstdio>
#include <core/Config.hpp>
#include <core/Engine.hpp>
#include <settings/SettingsData.hpp>
#include <settings/SettingsManager.hpp>
#include <settings/SettingsSerializer.hpp>
#include <components/CubeMap.hpp>
#include <iostream>

using namespace glm;

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Engine::Engine(void)
    : camera(WINDOW_WIDTH, WINDOW_HEIGHT, vec3(250.0f, 150.0f, -100.0f)),
      _isUIEnable(false), _lastFrame(0.0f), _deltaTime(0.0f), _frameCount(0) {
  _initGLFW();
  if (window.init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) == -1) {
    glfwTerminate();
    throw Engine::WindowInitFailedException();
  }
  _initGLAD();
  interface.init(window);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDepthFunc(GL_LESS);

  glPatchParameteri(GL_PATCH_VERTICES, 4);
}

Engine::~Engine() {
  window.destroy();
  interface.shutdown();
  glfwTerminate();
}

/* ========================================================================== */
/*                                    INIT                                    */
/* ========================================================================== */

void Engine::_initGLFW(void) const {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, false);
}

void Engine::_initGLAD(void) const {
  if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    window.close();
    glfwTerminate();
    throw Engine::GladLoadFailedException();
  }
}

/* ========================================================================== */
/*                                     RUN                                    */
/* ========================================================================== */

void Engine::run(void) {
  int waterSize = 128;
  int waterGridSize = 50;
  int waterCenter = waterSize * waterGridSize * 0.5;

  Water water(waterSize, waterSize, waterGridSize);
  water.init();
  camera.position = vec3(waterCenter, 100.0, waterCenter);

  Environment environment(WINDOW_WIDTH, WINDOW_HEIGHT);
  CubeMap& skybox = environment.skybox;
  vec3 skyColor = environment.skyColor;

  _loadSettings(water, environment);

  float lastTime = glfwGetTime();
  double ms = 0.0;
  char fpsText[100] = "Debug: 0 ms/frame";

  while (not window.shouldClose()) {
    glfwPollEvents();
    _handleInput();
    _updateDeltaTime();

    _frameCount++;
    if (glfwGetTime() - lastTime >= 1.0f) {
      sprintf(fpsText, "Debug: %.2f ms/frame\nWater Render: %.2f ms/frame",
              1000.0 / double(_frameCount), ms);
      _frameCount = 0;
      lastTime += 1.0f;
    }

    environment.attach();
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.updateMatrix(CAMERA_FOV, CAMERA_NEAR, CAMERA_FAR);

    if (not interface.wantCaptureMouse()) {
      camera.handleInput(window, _deltaTime);
    }

    // Isolate water rendering //
    GLuint queries[2];
    glGenQueries(2, queries);

    glQueryCounter(queries[0], GL_TIMESTAMP);
    water.render(camera, environment);
    glQueryCounter(queries[1], GL_TIMESTAMP);

    GLuint64 t0, t1;
    glGetQueryObjectui64v(queries[0], GL_QUERY_RESULT, &t0);
    glGetQueryObjectui64v(queries[1], GL_QUERY_RESULT, &t1);

    ms = (t1 - t0) / 1e6;
    // *********************** //

    skybox.render(camera, environment);

    environment.detach();
    environment.render(camera);

    if (_isUIEnable) {
      interface.createFrame();
      if (_settingsPanel.render(water, environment, fpsText)) {
        _saveSettings(water, environment);
      }
      interface.render();
    }
    window.update();
  }
}

/* ========================================================================== */
/*                                    INPUT                                   */
/* ========================================================================== */

void Engine::_handleInput(void) {
  if (window.isKeyPressed(GLFW_KEY_ESCAPE)) {
    window.close();
  }
  if (window.isKeyJustPressed(GLFW_KEY_T)) {
    _isUIEnable = not _isUIEnable;
  }
}

/* ========================================================================== */
/*                                   SETTINGS                                 */
/* ========================================================================== */

void Engine::_loadSettings(Water& water, Environment& environment) {
  setting::data settings;

  if (SettingsManager::load("settings.json", settings) == false) {
    std::cerr << "Warning: failed to load settings.json.\nDefault settings are "
                 "applied instead."
              << std::endl;
  }

  setting::applyAll(settings, water, environment);
}

void Engine::_saveSettings(const Water& water, const Environment& environment) {
  setting::data settings;

  setting::extractAll(settings, water, environment);

  if (SettingsManager::save("settings.json", settings) == false) {
    std::cerr << "Error: failed to save your settings.\n." << std::endl;
  } else {
    std::cout << "Settings successfully saved !" << std::endl;
  }
}

/* ========================================================================== */
/*                                    FRAME                                   */
/* ========================================================================== */

void Engine::_updateDeltaTime(void) {
  float currentFrame = glfwGetTime();

  _deltaTime = currentFrame - _lastFrame;
  _lastFrame = currentFrame;
}

/* ========================================================================== */
/*                                  EXCEPTION                                 */
/* ========================================================================== */

const char* Engine::WindowInitFailedException::what(void) const noexcept {
  return "Exception: failed while initiating the window";
}

const char* Engine::GladLoadFailedException::what(void) const noexcept {
  return "Exception: failed while loading GLAD";
}
