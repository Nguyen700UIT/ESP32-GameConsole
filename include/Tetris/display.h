#pragma once
#include <TFT_eSPI.h>

namespace tetris {

extern TFT_eSPI tft;
extern TFT_eSprite tetrisCanvas;
extern TFT_eSprite UICanvas;
void initDisplay();

}
