#pragma once
#include <TFT_eSPI.h>
#include "flappyBird/config.h"

namespace flappy_bird {

extern TFT_eSPI tft;
extern TFT_eSprite canvas;

void initDisplay();

}
