#include <effects/Fog.hpp>

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Fog::Fog(void)
    : enabled(false), near(0.1f), far(1000.0f), steepness(0.005f),
      offset(500.0f), color(0.75f) {}

Fog::~Fog() {}
