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

}
