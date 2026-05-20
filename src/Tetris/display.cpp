#include "Tetris/display.h"

namespace tetris {

TFT_eSPI tft;
TFT_eSprite tetrisCanvas = TFT_eSprite(&tft);


void initDisplay()
{
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tetrisCanvas.setColorDepth(8);
    tetrisCanvas.createSprite(200, 240);
}

}
