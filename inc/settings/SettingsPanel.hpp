#ifndef SETTINGS_PANEL_HPP
#define SETTINGS_PANEL_HPP

class Water;
class Environment;

class SettingsPanel {
public:
  bool render(Water& water, Environment& environment, const char* fpsText);
};

#endif
