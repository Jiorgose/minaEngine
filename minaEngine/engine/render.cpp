#include "render.hpp"

const int numColors = 32;
uint32_t colorPalette[33] = {
  0xFF181840, 0xFF604060, 0xFF846b63, 0xFFadb5bd,
  0xFFffffff, 0xFF8878d0, 0xFF98a8f8, 0xFF282882,
  0xFF3928ff, 0xFF4868e8, 0xFF425984, 0xFF3ca5cc,
  0xFF10ffff, 0xFF9040a8, 0xFFf82878, 0xFFf868c8,
  0xFFf890b8, 0xFF9c1842, 0xFFd63100, 0xFFff9c47,
  0xFFf8d820, 0xFF195a19, 0xFF10a500, 0xFF84ce42,
  0xFF94ffbd, 0xFF403000, 0xFFad6908, 0xFFd69221,
  0xFFe8b860, 0xFFffce8c, 0xFFce8263, 0xFFff9284,
  0x00000000
};

uint32_t vec4toColor(vec4 color) {
  uint8_t r = (uint8_t)color.r;
  uint8_t g = (uint8_t)color.g;
  uint8_t b = (uint8_t)color.b;
  uint8_t a = (uint8_t)color.a;

  return (a << 24) | (r << 16) | (g << 8) | b;
}

void render(uint32_t* buffer, int width, int height) {
  int stripeWidth = width / numColors;

  for (int x = 0; x < width; x++) {
    int colorIndex = x / stripeWidth;
    if (colorIndex >= numColors) colorIndex = numColors;

    for (int y = 0; y < height; y++) {
      int i = y * width + x;
      buffer[i] = colorPalette[colorIndex];
    }
  }
}
