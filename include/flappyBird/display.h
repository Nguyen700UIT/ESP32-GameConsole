#pragma once
#include <TFT_eSPI.h>
#include "flappyBird/config.h"

namespace flappy_bird {

extern TFT_eSPI tft;
extern TFT_eSprite canvas;

void initDisplay();
void drawBitmapTransparent(TFT_eSprite& canvas, int x, int y, int w, int h, const uint16_t *bitmap, u_int16_t transparent);

}
