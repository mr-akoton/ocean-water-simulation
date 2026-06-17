#include <core/Engine.hpp>
#include <settings.hpp>

using namespace glm;

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Engine::Engine(void)
    : camera(WINDOW_WIDTH, WINDOW_HEIGHT, vec3(250.0f, 150.0f, -100.0f)),
      _skyColor(0.0f), _lightDirection(-0.2f, -0.5f, -1.0f),
      _lightColor(0.769f, 0.775f, 0.680f), _lastFrame(0.0f), _deltaTime(0.0f),
      _frameCount(0) {
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

const std::vector<glm::vec4> PPVertices{
    {-1.0f, -1.0f, 0.0f, 0.0f}, // 1
    {+1.0f, -1.0f, 1.0f, 0.0f}, // 2
    {+1.0f, +1.0f, 1.0f, 1.0f}, // 3
    {-1.0f, -1.0f, 0.0f, 0.0f}, // 4
    {+1.0f, +1.0f, 1.0f, 1.0f}, // 5
    {-1.0f, +1.0f, 0.0f, 1.0f}, // 6
};

void Engine::run(void) {
  Water water(1000, 1000);
  water.init();

  Environment environment;
  CubeMap& skybox = environment.skybox;

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

    environment.attach();
    glClearColor(_skyColor.x, _skyColor.y, _skyColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.updateMatrix(45.0f, 0.1f, 2000.f);

    if (not interface.wantCaptureMouse()) {
      camera.handleInput(window, _deltaTime);
    }

    water.render(camera, environment.lightDirection, environment.lightColor);
    skybox.render(camera, environment.lightDirection, environment.lightColor,
                  _skyColor);

    environment.detach();
    environment.render(camera);

    _displayUI(water, environment, fpsText);
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

void Engine::_displayUI(Water& water, Environment& environment, char* fpsText) {
  interface.createFrame();
  ImGui::Begin("Setting");

  ImGui::TextUnformatted(fpsText);
  ImGui::Separator();

  if (ImGui::CollapsingHeader("Water", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::ColorEdit3("Color", value_ptr(water.color));

    ImGui::DragInt("Iterations", &water.iteration, 0.1f, 1, 100);

    ImGui::SliderFloat("Amplitude", &water.amplitude, 0.0f, 20.0f, "%.0f");
    ImGui::SliderFloat("Frequency", &water.frequency, 0.0f, 1.0f, "%.3f");
    ImGui::SliderFloat("Speed", &water.speed, 0.0f, 10.0f, "%.2f");
    ImGui::SliderFloat("Drag", &water.drag, 0.0f, 5.0f, "%.2f");

    ImGui::SeparatorText("Wave Shaping");
    ImGui::InputFloat("Peak Max", &water.peakMax);
    ImGui::InputFloat("Peak Offset", &water.peakOffset);

    ImGui::SeparatorText("Multipliers");
    ImGui::DragFloat("Amplitude Mult", &water.amplitudeMult, 0.01f, 0.0f,
                     10.0f);
    ImGui::DragFloat("Frequency Mult", &water.frequencyMult, 0.01f, 0.0f,
                     10.0f);
    ImGui::DragFloat("Speed Mult", &water.speedMult, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("Iteration Mult", &water.iterationMult, 0.01f, 0.0f,
                     10.0f);

    ImGui::SeparatorText("Materials");
    ImGui::ColorEdit3("Ambient Color", value_ptr(water.ambienColor));
    ImGui::SliderFloat("Ambient Strength", &water.ambientStrength, 0.0f, 1.0f,
                       "%.2f");
    ImGui::SliderFloat("Specular Strength", &water.specularStrength, 0.0f, 1.0f,
                       "%.2f");
    ImGui::InputInt("Shininess", &water.shininess);
  }

  if (ImGui::CollapsingHeader("Lighting")) {
    ImGui::ColorEdit3("Sky Color", value_ptr(_skyColor));
    ImGui::ColorEdit3("Light Color", value_ptr(environment.lightColor));
    ImGui::SliderFloat3("Direction", value_ptr(environment.lightDirection),
                        -1.0f, 1.0f);

    ImGui::Separator();
    ImGui::DragFloat("Sun Size", &environment.skybox.sunSize, 0.01f, 0.0f,
                     100.0f);
    ImGui::DragFloat("Sun Brightness", &environment.skybox.sunBrightness, 0.01f,
                     0.0f, 10.0f);
  }

  if (ImGui::CollapsingHeader("Fog")) {
    ImGui::Checkbox("Enable Fog", &environment.fog.enabled);
    ImGui::ColorEdit3("Fog Color", value_ptr(environment.fog.color));

    ImGui::DragFloat("Fog Near", &environment.fog.near, 0.1f, 0.0f, 1000.0f);
    ImGui::DragFloat("Fog Far", &environment.fog.far, 0.1f, 0.0f, 5000.0f);

    ImGui::InputFloat("Fog Steepness", &environment.fog.steepness);
    ImGui::InputFloat("Fog Offset", &environment.fog.offset);
  }

  ImGui::End();
  interface.render();
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
