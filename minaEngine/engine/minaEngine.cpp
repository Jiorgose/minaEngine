#include "minaEngine.hpp"

namespace minaEngine {
  

//STATICS
//-------------------------------------------------------------------
const int numColors = 33;
std::array<uint32_t, 33> colorPalette = {
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

static mfb_window* window = nullptr;
static std::vector<uint32_t> buffer;
const int width = 320;
const int height = 240;
//-------------------------------------------------------------------

//RENDER LOGIC
//-------------------------------------------------------------------
void render(uint32_t* buffer, int width, int height) {
  const int stripeWidth = width / numColors;

  for (int x = 0; x < width; x++) {
    int colorIndex = x / stripeWidth;
    if (colorIndex >= numColors) colorIndex = numColors - 1;

    for (int y = 0; y < height; y++) {
      int i = y * width + x;
      buffer[i] = colorPalette[colorIndex];
    }
  }
}
//-------------------------------------------------------------------

//WINDOW LOGIC
//-------------------------------------------------------------------
void setFullscreen(mfb_window* window, const int width, const int height) {
  float scaleX, scaleY;
  mfb_get_monitor_scale(window, &scaleX, &scaleY);

  float bufferAspect = (float)width / (float)height;
  int winWidth = (int)(mfb_get_window_width(window) / scaleX);
  int winHeight = (int)(mfb_get_window_height(window) / scaleY);
  float windowAspect = (float)winWidth / (float)winHeight;

  int newWidth, newHeight;

  if(windowAspect > bufferAspect) {
    newWidth = (int)(winHeight * bufferAspect);
    newHeight = winHeight;
  }
  else {
    newWidth = winWidth;
    newHeight = (int)(winWidth / bufferAspect);
  }

  int newX = (winWidth - newWidth) / 2;
  int newY = (winHeight - newHeight) / 2;

  mfb_set_viewport(window, newX, newY, newWidth, newHeight);
}
//-------------------------------------------------------------------

//HIGH LEVEL LOGIC
//-------------------------------------------------------------------
void init() {
  buffer.resize(width * height);

  window = mfb_open_ex("minaEngine", width, height, WF_FULLSCREEN);

  setFullscreen(window, width, height);
  mfb_set_target_fps(15);
}
bool shouldClose() {
  return mfb_wait_sync(window) == 0;
}
void update() {
  render(buffer.data(), width, height);
  mfb_update_ex(window, buffer.data(), width, height);

  int state = mfb_update_ex(window, buffer.data(), width, height);

  if(state < 0) {
    window = NULL;
    buffer.clear();
  }
}
//-------------------------------------------------------------------


}