#include "Snake/display.h"

namespace snake {

TFT_eSPI tft_snake;

void initDisplay()
{
    tft_snake.init();
    tft_snake.setRotation(1);
    tft_snake.fillScreen(TFT_BLACK);
}

}
