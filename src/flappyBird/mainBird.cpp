#include "apps/flappy_bird_app.h"
#include "flappyBird/audio.h"
#include "flappyBird/bird.h"
#include "flappyBird/config.h"
#include "flappyBird/display.h"
#include "flappyBird/flappyBird.h"
#include "flappyBird/tube.h"
#include <console/storage.h>


namespace flappy_bird {
namespace {
  int savedBestScore = 0;
  timeTheme theme;
}

void enter()
{
  prefs.begin("scores", false); //Not read only
  bestScore = prefs.getInt("bird", 0); //Default = 0, Read from flash
  savedBestScore = bestScore;
  initAudio();
  initDisplay();
  resetGame();
  theme = getTheme();
  renderGame(theme);
  reseted = false;
}

void tick()
{


  if (!gameOver) {
    if (isUp) {
      birdJumpLogic();
      isUp = false;
    }

    birdDropLogic();
    collisionLogic();
    scored();
    if (bestScore != savedBestScore)
    {
      savedBestScore = bestScore;
    }
    updateTubes();
    renderGame(theme);
    if (reseted) 
    {
      reseted = false;
    }
  }
  else {
    handleGameOver();
    if (reseted)
    {
      reseted = false;
      resetGame();
    }
  }
}

void exit()
{
  if (bestScore != savedBestScore)
  {
    prefs.putInt("bird", bestScore);
    savedBestScore = bestScore;
  }
  prefs.end();
  shutdownAudio();
  canvas.deleteSprite();
  tft.fillScreen(TFT_BLACK);
  isUp = false;
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
    case console::Button::Reset:
      reseted = true;
      break;
    case console::Button::Down:
    case console::Button::Left:
    case console::Button::Right:
      break;
  }
}

void onButtonReleased(console::Button button)
{
  (void)button;
}

}
