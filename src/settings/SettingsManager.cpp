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
           {"emissivity", s.emissivity},
           {"baseReflectance", s.baseReflectance},
           {"roughness", s.roughness},
           {"metallic", s.metallic},
           {"scatterColor", s.scatterColor},
           {"scatterStrength", s.scatterStrength},
           {"scatterPower", s.scatterPower},
           {"scatterDistortion", s.scatterDistortion},
           {"foamColor", s.foamColor},
           {"foamHeight", s.foamHeight},
           {"foamJacobian", s.foamJacobian},
           {"foamSteepness", s.foamSteepness},
           {"minDivision", s.minDivision},
           {"maxDivision", s.maxDivision},
           {"minDistance", s.minDistance},
           {"maxDistance", s.maxDistance},

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
  get_optional(j, "waterColor", s.waterColor);

  get_optional(j, "iterations", s.iteration);
  get_optional(j, "amplitude", s.amplitude);
  get_optional(j, "frequency", s.frequency);
  get_optional(j, "speed", s.speed);
  get_optional(j, "drag", s.drag);
  get_optional(j, "peakMax", s.peakMax);
  get_optional(j, "peakOffset", s.peakOffset);
  get_optional(j, "amplitudeMult", s.amplitudeMult);
  get_optional(j, "frequencyMult", s.frequencyMult);
  get_optional(j, "speedMult", s.speedMult);
  get_optional(j, "iterationMult", s.iterationMult);
  get_optional(j, "emissivity", s.emissivity);
  get_optional(j, "baseReflectance", s.baseReflectance);
  get_optional(j, "roughness", s.roughness);
  get_optional(j, "metallic", s.metallic);
  get_optional(j, "scatterColor", s.scatterColor);
  get_optional(j, "scatterStrength", s.scatterStrength);
  get_optional(j, "scatterPower", s.scatterPower);
  get_optional(j, "scatterDistortion", s.scatterDistortion);
  get_optional(j, "foamColor", s.foamColor);
  get_optional(j, "foamHeight", s.foamHeight);
  get_optional(j, "foamSteepness", s.foamSteepness);
  get_optional(j, "foamJacobian", s.foamJacobian);

  get_optional(j, "minDivision", s.minDivision);
  get_optional(j, "maxDivision", s.maxDivision);
  get_optional(j, "minDistance", s.minDistance);
  get_optional(j, "maxDistance", s.maxDistance);

  get_optional(j, "skyColor", s.skyColor);
  get_optional(j, "lightColor", s.lightColor);
  get_optional(j, "lightDirection", s.lightDirection);

  get_optional(j, "sunSize", s.sunSize);
  get_optional(j, "sunBrightness", s.sunBrightness);

  get_optional(j, "enableFog", s.enableFog);
  get_optional(j, "fogColor", s.fogColor);
  get_optional(j, "fogNear", s.fogNear);
  get_optional(j, "fogFar", s.fogFar);
  get_optional(j, "fogSteepness", s.fogSteepness);
  get_optional(j, "fogOffset", s.fogOffset);
}

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
