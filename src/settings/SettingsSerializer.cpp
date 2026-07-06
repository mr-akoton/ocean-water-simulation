#include <settings/SettingsSerializer.hpp>
#include <components/Water.hpp>
#include <components/Environment.hpp>

namespace setting {

void applyWater(const data& s, Water& water) {
  water.iteration = s.iteration;
  water.amplitude = s.amplitude;
  water.frequency = s.frequency;
  water.speed = s.speed;
  water.drag = s.drag;
  water.peakMax = s.peakMax;
  water.peakOffset = s.peakOffset;
  water.amplitudeMult = s.amplitudeMult;
  water.frequencyMult = s.frequencyMult;
  water.speedMult = s.speedMult;
  water.iterationMult = s.iterationMult;
  water.color = s.waterColor;
  water.emissivity = s.emissivity;
  water.baseReflectance = s.baseReflectance;
  water.roughness = s.roughness;
  water.metallic = s.metallic;
  water.scatterColor = s.scatterColor;
  water.scatterStrength = s.scatterStrength;
  water.scatterPower = s.scatterPower;
  water.scatterDistortion = s.scatterDistortion;
  water.foamColor = s.foamColor;
  water.foamHeight = s.foamHeight;
  water.foamSteepness = s.foamSteepness;
  water.foamJacobian = s.foamJacobian;
  water.minDivision = s.minDivision;
  water.maxDivision = s.maxDivision;
  water.minDistance = s.minDistance;
  water.maxDistance = s.maxDistance;
}

void extractWater(data& s, const Water& water) {
  s.iteration = water.iteration;
  s.amplitude = water.amplitude;
  s.frequency = water.frequency;
  s.speed = water.speed;
  s.drag = water.drag;
  s.peakMax = water.peakMax;
  s.peakOffset = water.peakOffset;
  s.amplitudeMult = water.amplitudeMult;
  s.frequencyMult = water.frequencyMult;
  s.speedMult = water.speedMult;
  s.iterationMult = water.iterationMult;
  s.waterColor = water.color;
  s.emissivity = water.emissivity;
  s.baseReflectance = water.baseReflectance;
  s.roughness = water.roughness;
  s.metallic = water.metallic;
  s.scatterColor = water.scatterColor;
  s.scatterStrength = water.scatterStrength;
  s.scatterPower = water.scatterPower;
  s.scatterDistortion = water.scatterDistortion;
  s.foamColor = water.foamColor;
  s.foamHeight = water.foamHeight;
  s.foamJacobian = water.foamJacobian;
  s.foamSteepness = water.foamSteepness;
  s.minDivision = water.minDivision;
  s.maxDivision = water.maxDivision;
  s.minDistance = water.minDistance;
  s.maxDistance = water.maxDistance;
}

void applyEnvironment(const data& s, Environment& env) {
  env.fog.enabled = s.enableFog;
  env.fog.color = s.fogColor;
  env.fog.far = s.fogFar;
  env.fog.near = s.fogNear;
  env.fog.offset = s.fogOffset;
  env.fog.steepness = s.fogSteepness;
  env.skybox.sunBrightness = s.sunBrightness;
  env.skybox.sunSize = s.sunSize;
  env.skyColor = s.skyColor;
  env.lightDirection = s.lightDirection;
  env.lightColor = s.lightColor;
}

void extractEnvironment(data& s, const Environment& env) {
  s.enableFog = env.fog.enabled;
  s.fogColor = env.fog.color;
  s.fogFar = env.fog.far;
  s.fogNear = env.fog.near;
  s.fogOffset = env.fog.offset;
  s.fogSteepness = env.fog.steepness;
  s.sunBrightness = env.skybox.sunBrightness;
  s.sunSize = env.skybox.sunSize;
  s.skyColor = env.skyColor;
  s.lightDirection = env.lightDirection;
  s.lightColor = env.lightColor;
}

void applyAll(const data& s, Water& water, Environment& env) {
  applyWater(s, water);
  applyEnvironment(s, env);
}

void extractAll(data& s, const Water& water, const Environment& env) {
  extractWater(s, water);
  extractEnvironment(s, env);
}

} // namespace setting
