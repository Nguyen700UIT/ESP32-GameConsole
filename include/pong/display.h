#pragma once
#include <TFT_eSPI.h>

namespace pong {

extern TFT_eSPI tft;
extern TFT_eSprite canvas;
extern bool canvasReady;

void initDisplay();

}
