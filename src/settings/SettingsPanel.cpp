#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <core/Config.hpp>
#include <components/Water.hpp>
#include <components/Environment.hpp>
#include <settings/SettingsPanel.hpp>

bool SettingsPanel::render(Water& water, Environment& environment,
                           const char* fpsText) {
  bool saveRequested = false;

  ImGui::Begin("Setting");

  ImGui::TextUnformatted(fpsText);
  if (ImGui::Button("Save")) {
    saveRequested = true;
  }

  ImGui::Separator();

  if (ImGui::BeginTabBar("SettingsTabs", ImGuiTabBarFlags_None)) {
    if (ImGui::BeginTabItem("Water")) {
      ImGui::ColorEdit3("Water Color", glm::value_ptr(water.color));
      ImGui::DragInt("Iterations", &water.iteration, 1, 1, MAX_WAVE_ITERATION);

      if (ImGui::CollapsingHeader("Wave Parameters",
                                  ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderFloat("Amplitude", &water.amplitude, 0.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Frequency", &water.frequency, 0.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Speed", &water.speed, 0.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Drag", &water.drag, 0.0f, 5.0f, "%.2f");
      }

      if (ImGui::CollapsingHeader("Wave Shaping",
                                  ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputFloat("Peak Max", &water.peakMax);
        ImGui::InputFloat("Peak Offset", &water.peakOffset);
      }

      if (ImGui::CollapsingHeader("Multipliers")) {
        ImGui::DragFloat("Amplitude Mult", &water.amplitudeMult, 0.01f, 0.0f,
                         10.0f);
        ImGui::DragFloat("Frequency Mult", &water.frequencyMult, 0.01f, 0.0f,
                         10.0f);
        ImGui::DragFloat("Speed Mult", &water.speedMult, 0.01f, 0.0f, 10.0f);
      }

      if (ImGui::CollapsingHeader("Materials")) {
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(water.ambientColor));
        ImGui::SliderFloat("Ambient Strength", &water.ambientStrength, 0.0f,
                           1.0f, "%.2f");
        ImGui::SliderFloat("Specular Strength", &water.specularStrength, 0.0f,
                           1.0f, "%.2f");
        ImGui::InputInt("Shininess", &water.shininess);
      }

      if (ImGui::CollapsingHeader("Tessellation")) {
        ImGui::InputFloat("Min Division", &water.minDivision);
        ImGui::InputFloat("Max Division", &water.maxDivision);
        ImGui::InputFloat("Min Distance", &water.minDistance);
        ImGui::InputFloat("Max Distance", &water.maxDistance);
      }

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Lighting")) {
      ImGui::ColorEdit3("Sky Color", glm::value_ptr(environment.skyColor));
      ImGui::ColorEdit3("Light Color", glm::value_ptr(environment.lightColor));
      ImGui::SliderFloat3(
          "Direction", glm::value_ptr(environment.lightDirection), -1.0f, 1.0f);

      ImGui::Separator();

      ImGui::DragFloat("Sun Size", &environment.skybox.sunSize, 0.01f, 0.0f,
                       100.0f);
      ImGui::DragFloat("Sun Brightness", &environment.skybox.sunBrightness,
                       0.01f, 0.0f, 10.0f);

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Fog")) {
      ImGui::Checkbox("Enable Fog", &environment.fog.enabled);
      ImGui::ColorEdit3("Fog Color", glm::value_ptr(environment.fog.color));

      ImGui::Separator();

      ImGui::DragFloat("Fog Near", &environment.fog.near, 0.1f, 0.0f, 1000.0f);
      ImGui::DragFloat("Fog Far", &environment.fog.far, 0.1f, 0.0f, 5000.0f);

      ImGui::InputFloat("Fog Steepness", &environment.fog.steepness);
      ImGui::InputFloat("Fog Offset", &environment.fog.offset);

      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  ImGui::End();

  return saveRequested;
}
