#include "engine/minaEngine.hpp"

int main() {
  init();

  loadScene("pong");

  object& paddleG = addObject(object(vec4(320, 0, 16, 96), vec2(10, 120), vec2(16, 96), 0.0f, 0));
  object& paddleR = addObject(object(vec4(336, 0, 16, 96), vec2(310, 120), vec2(16, 96), 0.0f, 0));
  object& ball = addObject(object(vec4(320, 96, 32, 32), vec2(160, 120), vec2(32), 0.0f, 1));
  
  while(!shouldClose()) {
    time gameTime = getTime();

    ball.rotation = gameTime.totalTime * 25.0f * gameTime.deltaTime;

    update();
  }

  removeObject(paddleG);
  removeObject(paddleR);
  removeObject(ball);
}