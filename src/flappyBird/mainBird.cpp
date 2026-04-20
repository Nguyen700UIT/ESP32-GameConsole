#include "apps/flappy_bird_app.h"
#include "flappyBird/audio.h"
#include "flappyBird/bird.h"
#include "flappyBird/config.h"
#include "flappyBird/display.h"
#include "flappyBird/flappyBird.h"
#include "flappyBird/tube.h"
#include <console/storage.h>


namespace flappy_bird {
void enter()
{
  prefs.begin("scores", false); //Not read only
  bestScore = prefs.getInt("bird", 0); //Default = 0, Read from flash
  initAudio();
  initDisplay();
  resetGame();
  reseted = false;
}

void tick()
{
  if (reseted) {
    resetGame();
    reseted = false;
  }

  if (!gameOver) {
    if (isUp) {
      birdJumpLogic();
      isUp = false;
    }

    birdDropLogic();
    collisionLogic();
    scored();
    prefs.putInt("bird", bestScore);
    updateTubes();
    renderGame();
  }
  else {
    handleGameOver();
  }
}

void exit()
{
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

}
