#pragma once
#include <cstdint>
#include <array>
#include <string>
#include <algorithm>
#include <MiniFB.h>
#include <glm/glm.hpp>
using namespace glm;

struct object {
    vec4 sprite;
    vec2 position;
    vec2 size;
    float rotation;
};

inline bool operator==(const object& lhs, const object& rhs) {
    return lhs.sprite == rhs.sprite
        && lhs.position == rhs.position
        && lhs.size == rhs.size
        && lhs.rotation == rhs.rotation;
}

struct Scene {
  std::vector<object> objects;
  std::vector<uint8_t> atlas;
};

void init();
bool shouldClose();
void update();
void loadScene(const char* sceneName);
object& addObject(const object& obj);
void removeObject(const object& objectToRemove);
