#include "apps/pong_app.h"
#include "pong/config.h"
#include "pong/display.h"
#include "pong/pong.h"
#include "pong/ai_interference.h"
#include "pong/audio.h"

namespace pong {

namespace {
    console::FpsCounter fpsCounter;
    unsigned long lastFpsDrawAt = 0;
    constexpr unsigned long kFpsDrawIntervalMs = 250;

    void resetFpsCounter()
    {
        fpsCounter.reset(millis());
        lastFpsDrawAt = 0;
    }

    void recordFpsFrame()
    {
        unsigned long now = millis();
        fpsCounter.recordFrame(now);
        if (fpsCounter.hasStats() && now - lastFpsDrawAt >= kFpsDrawIntervalMs)
        {
            drawFpsStats(fpsCounter.stats());
            lastFpsDrawAt = now;
        }
    }
}

void enter()
{
    initAudio();
    initDisplay();
    initAI();
    resetGame();
    resetFpsCounter();
    render();
    recordFpsFrame();
    isUp = false;
    isDown = false;
    reseted = false;
}

void tick()
{
    if (!gameOverFlag)
    {
        if (reseted)
            reseted = false;
        gameLogicForAIML();
        render();
        recordFpsFrame();
    }
    else
    {
        drawGameOver();
        if (reseted)
        {
            tft.fillScreen(TFT_BLACK);
            resetGame();
            resetFpsCounter();
            render();
            recordFpsFrame();
            reseted = false;
        }
    }
}

void exit()
{
    if (canvasReady)
    {
        canvas.deleteSprite();
        canvasReady = false;
    }
    shutdownAudio();
    releaseAI();
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
            if (!gameOverFlag)
            {
                isUp = true;
            }
            break;
        case console::Button::Down:
            if (!gameOverFlag)
            {
                isDown = true;
            }
            break;
        case console::Button::Reset:
            reseted = true;
            break;
        case console::Button::Left:
            break;
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
