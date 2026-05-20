#include "flappyBird/display.h"

namespace flappy_bird {

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite canvas = TFT_eSprite(&tft);

void initDisplay()
{
  tft.init();
  tft.setRotation(1);
  canvas.setColorDepth(8);
  canvas.createSprite(SCREEN_WIDTH, GAME_HEIGHT);
}

void drawBitmapTransparent(TFT_eSprite& canvasParam, int x, int y, int w, int h, const uint16_t *bitmap, u_int16_t transparent)
{
  for (int py = 0; py < h; ++py)
  {
    for (int px = 0; px < w; ++px)
    {
      uint16_t color = pgm_read_word(&bitmap[py * w + px]);
      if (color != transparent)
        canvasParam.drawPixel(x + px, y + py, color);
  }
  }
}

}