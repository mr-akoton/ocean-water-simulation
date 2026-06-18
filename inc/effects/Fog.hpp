#ifndef FOG_HPP
#define FOG_HPP

#include <glm/glm.hpp>

class Fog {
public:
  bool enabled;
  float near;
  float far;
  float steepness;
  float offset;
  glm::vec3 color;

public:
  Fog(void);
  ~Fog();
};

#endif
