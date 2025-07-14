#include "engine/minaEngine.hpp"

int main() {
  init();

  loadScene("pong");

  object& paddleG = addObject(object(vec4(320, 0, 16, 96), vec2(50, 50), vec2(16, 96), radians(45.0f)));
  
  while(!shouldClose()) {
    update();
  }

  removeObject(paddleG);
}