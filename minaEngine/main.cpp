#include "engine/minaEngine.hpp"

int main() {
  minaEngine::init();

  while(!minaEngine::shouldClose()) {
    minaEngine::update();
  }
}