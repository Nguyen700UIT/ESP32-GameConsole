#include "pong/display.h"
#include "pong/config.h"

namespace pong {

TFT_eSPI tft;
TFT_eSprite canvas = TFT_eSprite(&tft);
bool canvasReady = false;

void initDisplay()
{
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    canvas.setColorDepth(8);
    canvasReady = (canvas.createSprite(SCREEN_WIDTH, GAME_HEIGHT) != nullptr);
}

}
