#include <MiniFB.h>
#include <vector>

#include "fullscreen.hpp"
#include "render.hpp"

const int width = 320;
const int height = 240;

int main() {
  struct mfb_window* window = mfb_open_ex("minaEngine", width, height, WF_FULLSCREEN);
  setFullscreen(window, width, height);

  std::vector<uint32_t> buffer(width * height);

  while(mfb_wait_sync(window)) {
    render(buffer.data(), width, height);

    int state = mfb_update_ex(window, buffer.data(), width, height);

    if(state < 0) {
      window = NULL;
      break;
    }
  }
}