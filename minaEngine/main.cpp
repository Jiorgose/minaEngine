#include <MiniFB.h>
#include <vector>
#include <iostream>

const int width = 320;
const int height = 240;

void setFullscreen(mfb_window* window) {
  float scaleX, scaleY;
  mfb_get_monitor_scale(window, &scaleX, &scaleY);

  float bufferAspect = (float)width / (float)height;
  int winWidth = (int)(mfb_get_window_width(window) / scaleX);
  int winHeight = (int)(mfb_get_window_height(window) / scaleY);
  float windowAspect = (float)winWidth / (float)winHeight;

  int newWidth, newHeight;

  if (windowAspect > bufferAspect) {
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

int main() {
  struct mfb_window* window = mfb_open_ex("minaEngine", width, height, WF_FULLSCREEN);
  if (!window) {
    printf("Failed to open window\n");
    return -1;
  }
  setFullscreen(window);

  std::vector<uint32_t> buffer(width * height);
  std::fill(buffer.begin(), buffer.end(), 0xFFFF0000);

  while(mfb_wait_sync(window)) {
    int state = mfb_update_ex(window, buffer.data(), width, height);

    if(state < 0) {
      window = NULL;
      break;
    }
  }
}