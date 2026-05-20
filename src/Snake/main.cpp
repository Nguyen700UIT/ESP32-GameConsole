#include "apps/snake_app.h"
#include "Snake/config.h"
#include "Snake/display.h"
#include "Snake/snake_game.h"
#include "Snake/audio.h"
#include <console/storage.h>



namespace snake {

namespace{
    snakeColor color;
}
void enter()
{
    prefs.begin("scores", false); //Not read only
    bestScore = prefs.getInt("snake", 0); //Default = 0, Read from flash
    initAudio();
    initDisplay();
    gameReset();
    color = getColor();
    isUp = false;
    isDown = false;
    isLeft = false;
    isRight = false;
    reseted = false;
    
}

void tick()
{
    currTime = millis();

    if (!gameOver)
    {
        if (reseted)
        {
            reseted = false;
        }

        if (currTime - prevTime > moveDelay)
        {
            prevTime = currTime;

            readDirection();
            move();
            bool ate = eat();
            isGameOver();
            if (!gameOver)
            {
                score = (nTail - 1) * 10;
                if(score > bestScore)
                {
                    bestScore = score; //Update score
                }
                if (score != prevScore)
                {
                    drawScore();
                    prevScore = score;
                }

                renderBoard(ate, color);
            }
        }
    }
    else
    {
        drawGameOverUI();
        if (reseted)
        {
            gameReset();
            color = getColor();
            reseted = false;
        }
    }
}

void exit()
{
    shutdownAudio();
    prefs.putInt("snake", bestScore); //Store in flash
    prefs.end();
    tft_snake.fillScreen(TFT_BLACK);
    isUp = false;
    isDown = false;
    isLeft = false;
    isRight = false;
    reseted = false;
}

void onButtonPressed(console::Button button)
{
    switch (button)
    {
        case console::Button::Up:
            if (!gameOver)
            {
                isUp = true;
            }
            break;
        case console::Button::Down:
            if (!gameOver)
            {
                isDown = true;
            }
            break;
        case console::Button::Left:
            if (!gameOver)
            {
                isLeft = true;
            }
            break;
        case console::Button::Right:
            if (!gameOver)
            {
                isRight = true;
            }
            break;
        case console::Button::Reset:
            reseted = true;
            break;
    }
}

void onButtonReleased(console::Button button)
{
    (void)button;
}

}
