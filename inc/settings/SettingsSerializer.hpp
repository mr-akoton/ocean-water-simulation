#ifndef SETTINGS_SERIALIZER_HPP
#define SETTINGS_SERIALIZER_HPP

#include <settings/SettingsData.hpp>

class Water;
class Environment;

namespace setting {

void applyAll(const data& settings, Water& water, Environment& environment);
void extractAll(data& settings, const Water& water, const Environment& environment);

void applyWater(const data& settings, Water& water);
void extractWater(data& settings, const Water& water);

void applyEnvironment(const data& settings, Environment& env);
void extractEnvironment(data& settings, const Environment& env);

}

#endif
