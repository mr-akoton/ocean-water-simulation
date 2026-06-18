#ifndef SETTINGS_MANAGER_HPP
#define SETTINGS_MANAGER_HPP

#include <string>
#include <settings/SettingsData.hpp>

class SettingsManager {
public:
  static bool save(const std::string& filepath, const setting::data& settings);
  static bool load(const std::string& filepath, setting::data& settings);
};

#endif
