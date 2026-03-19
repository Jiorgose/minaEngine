#pragma once
#include <cstdint>
#include <array>
#include <string>
#include <algorithm>
#include <list>
#include <MiniFB.h>
#include <glm/glm.hpp>
using namespace glm;

struct object {
  vec4 sprite;
  vec2 position;
  vec2 size;
  float rotation;
  int layer;

  object(vec4 sprite, vec2 position, vec2 size, float rotation, int layer) : sprite(sprite), position(position), size(size), rotation(rotation), layer(layer) {}
};

inline bool operator==(const object& lhs, const object& rhs) {
  return lhs.sprite == rhs.sprite
    && lhs.position == rhs.position
    && lhs.size == rhs.size
    && lhs.rotation == rhs.rotation;
}

struct scene {
  std::list<object> objects;
  std::vector<uint8_t> atlas;
};

struct globalTime {
    float deltaTime = 0.0f;
    float totalTime = 0.0f;
    float updateTime = 0.0f;
};

void init();
bool shouldClose();
void update();
void loadScene(const char* sceneName);
void updateLayers();
object& addObject(const object& obj);
void removeObject(const object& objectToRemove);
globalTime getTime();