#include "components/Water.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include <core/Engine.hpp>
#include <components/CubeMap.hpp>

using namespace glm;

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Engine::Engine(void)
    : camera(WINDOW_WIDTH, WINDOW_HEIGHT, vec3(250.0f, 150.0f, -100.0f)),
      _lastFrame(0.0f), _deltaTime(0.0f), _frameCount(0) {

  _initGLFW();
  if (window.init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE) == -1) {
    glfwTerminate();
    throw Engine::WindowInitFailedException();
  }
  _initGLAD();
  interface.init(window);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
  Water water(500, 500);
  water.init(vec3(0.172f, 0.3412f, 0.4667f));

  CubeMap skybox((const char**)SKYBOX_FACES);

  vec3 lightDirection(-0.2f, -0.8f, -1.0f);
  vec3 lightColor(1.0f, 1.0f, 1.0f);

  float lastTime = glfwGetTime();
  char fpsText[100] = "Debug: 0 ms/frame";

  while (not window.shouldClose()) {
    glfwPollEvents();
    _handleInput();
    _updateDeltaTime();

    _frameCount++;
    if (glfwGetTime() - lastTime >= 1.0f) {
      sprintf(fpsText, "Debug: %f ms/frame", 1000.0 / double(_frameCount));
      _frameCount = 0;
      lastTime += 1.0f;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.updateMatrix(45.0f, 0.1f, 1000.0f);

    if (not interface.wantCaptureMouse()) {
      camera.handleInput(window, _deltaTime);
    }

    water.render(camera, lightDirection, lightColor);
    skybox.render(camera);

    interface.createFrame();
    ImGui::Begin("Setting");

    ImGui::Text("%s", fpsText);

    ImGui::Text("Water");
    ImGui::InputInt("Iteration", &water.iteration);
    ImGui::InputFloat("Amplitude", &water.amplitude);
    ImGui::InputFloat("Frequence", &water.frequency);
    ImGui::SliderFloat("Speed", &water.speed, 0.0, 10.0, "%.2f");
    ImGui::SliderFloat("Drag", &water.drag, -10.0, 10.0, "%.2f");
    ImGui::InputFloat("Peak Max", &water.peakMax);
    ImGui::InputFloat("Peak Offset", &water.peakOffset);

    ImGui::InputFloat("Amplitude Multiplier", &water.amplitudeMult);
    ImGui::InputFloat("Frequency Multiplier", &water.frequencyMult);
    ImGui::InputFloat("Speed Multiplier", &water.speedMult);
    ImGui::InputFloat("Iteration Multiplier", &water.iterationMult);

    ImGui::Text("Light");
    ImGui::SliderFloat3("Direction", value_ptr(lightDirection), -1, 1, "%.3f");
    ImGui::ColorEdit3("Color", value_ptr(lightColor));

    ImGui::End();
    interface.render();

    window.update();
  }
}

/* ========================================================================== */
/*                                    INPUT                                   */
/* ========================================================================== */

void Engine::_handleInput(void) const {
  if (window.isKeyPressed(GLFW_KEY_ESCAPE)) {
    window.close();
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
