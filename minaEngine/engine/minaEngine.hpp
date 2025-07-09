#pragma once
#include <cstdint>
#include <array>
#include <MiniFB.h>
#include <glm/glm.hpp>
using namespace glm;

namespace minaEngine {
  void init();
  bool shouldClose();
  void update();
}