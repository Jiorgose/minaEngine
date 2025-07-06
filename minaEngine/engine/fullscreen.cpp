#include "fullscreen.hpp"

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