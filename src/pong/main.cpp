#include "apps/pong_app.h"
#include "pong/config.h"
#include "pong/display.h"
#include "pong/pong.h"

namespace pong {

void enter()
{
    initDisplay();
    resetGame();
    render();
    isUp = false;
    isDown = false;
    reseted = false;
}

void tick()
{
//     if (!gameOverFlag)
//     {
        gameLogicForHumanML();
        render();
    // }
    // else
    // {
    //     drawGameOver();
    //     if (reseted)
    //     {
    //         tft.fillScreen(TFT_BLACK);
    //         resetGame();
    //         render();
    //         reseted = false;
    //     }
    // }
}

void exit()
{
    if (canvasReady)
    {
        canvas.deleteSprite();
        canvasReady = false;
    }
    tft.fillScreen(TFT_BLACK);
    isUp = false;
    isDown = false;
    reseted = false;
}

void onButtonPressed(console::Button button)
{
    switch (button)
    {
        case console::Button::Up:
            // if (!gameOverFlag)
            // {
                isUp = true;
            // }
            break;
        case console::Button::Down:
            // if (!gameOverFlag)
            // {
                isDown = true;
            // }
            break;
        case console::Button::Reset:
            reseted = true;
            break;
        case console::Button::Left:
        case console::Button::Right:
            break;
    }
}

void onButtonReleased(console::Button button)
{
  switch (button)
  {
    case console::Button::Down:
      isDown = false;
      break;
    case console::Button::Up:
      isUp = false;
      break;
    case console::Button::Left:
    case console::Button::Right:
    case console::Button::Reset:
      break;

}
}

}
