#ifndef VISUAL_EFFECTS_HPP
#define VISUAL_EFFECTS_HPP

#include <glm/glm.hpp>

struct Fog {
  glm::vec3 color;
  float near;
  float far;
  float steepnees;
  float offset;
};

#endif
