#include "minaEngine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//-------------------------------------------------------------------
//STATICS
//-------------------------------------------------------------------
const int numColors = 33;
std::array<uint32_t, numColors> colorPalette = {
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

const int width = 320;
const int height = 240;
const int atlasResolution = 512;
const int atlasSize = atlasResolution * atlasResolution;
static mfb_window* window = nullptr;
static std::vector<uint32_t> buffer;
static Scene currentScene;
static float globalTime = 0.0f;

//-------------------------------------------------------------------


//-------------------------------------------------------------------
//RENDER LOGIC
//-------------------------------------------------------------------
std::vector<uint8_t> loadAtlas(const char* fileName) {
  std::vector<uint8_t> result;

  int imageWidth, imageHeight, channels;
  unsigned char* image = stbi_load(fileName, &imageWidth, &imageHeight, &channels, 3);
  if (!image) return result;

  result.resize(atlasSize);

  std::array<glm::u8vec3, numColors> paletteRGB;
  for (int i = 0; i < numColors; ++i) {
    uint32_t color = colorPalette[i];
    paletteRGB[i] = glm::u8vec3((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
  }

  for (int i = 0; i < atlasSize; ++i) {
    glm::u8vec3 pixel(image[i * 3 + 0], image[i * 3 + 1], image[i * 3 + 2]);

    uint8_t paletteIndex = numColors - 1;
    for (uint8_t j = 0; j < numColors; ++j) {
      if (pixel == paletteRGB[j]) {
        paletteIndex = j;
        break;
      }
    }

    result[i] = paletteIndex;
  }

  stbi_image_free(image);
  return result;
}

uint32_t sampleSprite(vec2 samplePos, const object& obj) {
  vec2 center = obj.size * 0.5f;
  vec2 local = samplePos - center;

  float cosTheta = cos(-obj.rotation);
  float sinTheta = sin(-obj.rotation);

  vec2 rotated;
  rotated.x = local.x * cosTheta - local.y * sinTheta;
  rotated.y = local.x * sinTheta + local.y * cosTheta;

  rotated += center;

  if (rotated.x < 0.0f || rotated.x >= obj.size.x || rotated.y < 0.0f || rotated.y >= obj.size.y) {
    return 0x00000000;
  }

  vec2 spriteSize(obj.sprite.z, obj.sprite.w);
  vec2 uv = rotated / obj.size;
  vec2 atlasPos = vec2(obj.sprite.x, obj.sprite.y) + uv * spriteSize;

  int spriteX = int(atlasPos.x);
  int spriteY = int(atlasPos.y);

  if (spriteX < 0 || spriteX >= atlasResolution || spriteY < 0 || spriteY >= atlasResolution) {
    return 0x00000000;
  }

  int atlasIndex = spriteY * atlasResolution + spriteX;
  uint8_t paletteIndex = currentScene.atlas[atlasIndex];

  return colorPalette[paletteIndex];
}

void render(uint32_t* buffer, const int width, const int height) {
  //render background
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      int i = y * width + x;
      buffer[i] = colorPalette[currentScene.atlas[y * atlasResolution + x]];
    }
  }

  //render objects
  for (const object& oo : currentScene.objects) {
    float maxDim = length(oo.size);
    int renderSize = int(ceil(maxDim));

    for (int y = -renderSize / 2; y < renderSize / 2; y++) {
      for (int x = -renderSize / 2; x < renderSize / 2; x++) {
        vec2 samplePos = vec2(x, y) + oo.size * 0.5f;
        uint32_t color = sampleSprite(samplePos, oo);

        if (color == 0x00000000) {
          continue;
        }

        int worldX = int(oo.position.x) + x + renderSize / 2;
        int worldY = int(oo.position.y) + y + renderSize / 2;

        if (worldX < 0 || worldX >= width || worldY < 0 || worldY >= height) {
          continue;
        }

        buffer[worldY * width + worldX] = color;
      }
    }
  }
}
//-------------------------------------------------------------------


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
  globalTime += 1.0f / 15.0f;

  render(buffer.data(), width, height);

  int state = mfb_update_ex(window, buffer.data(), width, height);

  if(state < 0) {
    window = NULL;
    buffer.clear();
  }
}

void loadScene(const char* sceneName) {
  currentScene.atlas = loadAtlas((std::string("assets/") + sceneName + ".png").c_str());
}

object& addObject(const object& obj) {
  currentScene.objects.push_back(obj);
  return currentScene.objects.back();
}

void removeObject(const object& objectToRemove) {
  auto obj = std::find(currentScene.objects.begin(), currentScene.objects.end(), objectToRemove);
  if (obj != currentScene.objects.end()) {
    currentScene.objects.erase(obj);
  }
}
//-------------------------------------------------------------------