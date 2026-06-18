#ifndef SETTINGS_DATA_HPP
#define SETTINGS_DATA_HPP

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr const char* WINDOW_TITLE = "OpenGL - Water";
constexpr const char* SKYBOX_FACES[6] = {
    "resources/right.jpg",  "resources/left.jpg",  "resources/top.jpg",
    "resources/bottom.jpg", "resources/front.jpg", "resources/back.jpg"};

constexpr float CAMERA_FOV = 60.0f;
constexpr float CAMERA_NEAR = 0.1f;
constexpr float CAMERA_FAR = 2000.0f;

namespace glm {
void to_json(nlohmann::json& j, const glm::vec3& v);
void from_json(const nlohmann::json& j, glm::vec3& v);
}; // namespace glm

namespace setting {
struct data {
  // Water parameters
  glm::vec3 waterColor{0.629f, 0.883f, 0.917f};
  int iteration{32};
  float amplitude{8.0f};
  float frequency{0.02f};
  float speed{1.5f};
  float drag{0.5f};
  float peakMax{1.0f};
  float peakOffset{1.0f};
  float amplitudeMult{0.82f};
  float frequencyMult{1.18f};
  float speedMult{1.07f};
  float iterationMult{1.18f};
  glm::vec3 ambientColor{1.0f, 1.0f, 1.0f};
  float ambientStrength{0.4f};
  float specularStrength{1.0f};
  int shininess{256};

  // Environment
  glm::vec3 skyColor{0.0f, 0.0f, 0.0f};
  glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
  glm::vec3 lightDirection{-0.2f, -0.2f, -1.0f};

  // Cubemap
  float sunSize{0.999f};
  float sunBrightness{1.5f};

  // Fog
  bool enableFog{false};
  glm::vec3 fogColor{0.75f, 0.75f, 0.75f};
  float fogNear{0.1f};
  float fogFar{1000.f};
  float fogSteepness{0.005f};
  float fogOffset{500.0f};
};

void to_json(nlohmann::json& j, const data& s);
void from_json(const nlohmann::json& j, data& s);
}; // namespace setting

#endif
