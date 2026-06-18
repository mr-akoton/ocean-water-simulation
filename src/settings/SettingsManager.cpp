#include <settings/SettingsManager.hpp>
#include <nlohmann/json.hpp>

#include <fstream>

using json = nlohmann::json;

void setting::to_json(nlohmann::json& j, const setting::data& s) {
  j = json{{"waterColor", s.waterColor},
           {"iterations", s.iteration},
           {"amplitude", s.amplitude},
           {"frequency", s.frequency},
           {"speed", s.speed},
           {"drag", s.drag},
           {"peakMax", s.peakMax},
           {"peakOffset", s.peakOffset},
           {"amplitudeMult", s.amplitudeMult},
           {"frequencyMult", s.frequencyMult},
           {"speedMult", s.speedMult},
           {"iterationMult", s.iterationMult},
           {"ambientColor", s.ambientColor},
           {"ambientStrength", s.ambientStrength},
           {"specularStrength", s.specularStrength},
           {"shininess", s.shininess},

           {"skyColor", s.skyColor},
           {"lightColor", s.lightColor},
           {"lightDirection", s.lightDirection},

           {"sunSize", s.sunSize},
           {"sunBrightness", s.sunBrightness},

           {"enableFog", s.enableFog},
           {"fogColor", s.fogColor},
           {"fogNear", s.fogNear},
           {"fogFar", s.fogFar},
           {"fogSteepness", s.fogSteepness},
           {"fogOffset", s.fogOffset}};
};

void setting::from_json(const nlohmann::json& j, setting::data& s) {
  j.at("waterColor").get_to(s.waterColor);
  j.at("iterations").get_to(s.iteration);
  j.at("amplitude").get_to(s.amplitude);
  j.at("frequency").get_to(s.frequency);
  j.at("speed").get_to(s.speed);
  j.at("drag").get_to(s.drag);
  j.at("peakMax").get_to(s.peakMax);
  j.at("peakOffset").get_to(s.peakOffset);
  j.at("amplitudeMult").get_to(s.amplitudeMult);
  j.at("frequencyMult").get_to(s.frequencyMult);
  j.at("speedMult").get_to(s.speedMult);
  j.at("iterationMult").get_to(s.iterationMult);
  j.at("ambientColor").get_to(s.ambientColor);
  j.at("ambientStrength").get_to(s.ambientStrength);
  j.at("specularStrength").get_to(s.specularStrength);
  j.at("shininess").get_to(s.shininess);

  j.at("skyColor").get_to(s.skyColor);
  j.at("lightColor").get_to(s.lightColor);
  j.at("lightDirection").get_to(s.lightDirection);

  j.at("sunSize").get_to(s.sunSize);
  j.at("sunBrightness").get_to(s.sunBrightness);

  j.at("enableFog").get_to(s.enableFog);
  j.at("fogColor").get_to(s.fogColor);
  j.at("fogNear").get_to(s.fogNear);
  j.at("fogFar").get_to(s.fogFar);
  j.at("fogSteepness").get_to(s.fogSteepness);
  j.at("fogOffset").get_to(s.fogOffset);
};

bool SettingsManager::save(const std::string& filepath,
                           const setting::data& settings) {
  std::ofstream file(filepath);
  if (!file.is_open())
    return false;

  json j = settings;
  file << j.dump(4);
  return true;
}

bool SettingsManager::load(const std::string& filepath,
                           setting::data& settings) {
  std::ifstream file(filepath);
  if (!file.is_open())
    return false;

  json j;
  file >> j;
  settings = j.get<setting::data>();
  return true;
}
