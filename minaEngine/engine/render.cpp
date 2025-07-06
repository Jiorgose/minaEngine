#include "render.hpp"

uint32_t vec4toColor(vec4 color) {
  uint8_t r = (uint8_t)color.r;
  uint8_t g = (uint8_t)color.g;
  uint8_t b = (uint8_t)color.b;
  uint8_t a = (uint8_t)color.a;

  return (a << 24) | (r << 16) | (g << 8) | b;
}

void render(uint32_t* buffer, int width, int height) {
  std::fill(buffer, buffer + (width * height), 0xFF000000);
  int totalPixels = width * height;

  for(int x = 0; x < width; x++) {
    for(int y = 0; y < height; y++) {
      int i = y * width + x;

      float u = ((float)x / (width - 1)) * 255.0f;
      float v = ((float)(height - 1 - y) / (height - 1)) * 255.0f;

      buffer[i] = vec4toColor(vec4(u, v, 0.0f, 255.0f));
    }
  }
}
