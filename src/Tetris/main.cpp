#include "apps/tetris_app.h"
#include "Tetris/audio.h"
#include "Tetris/config.h"
#include "Tetris/display.h"
#include "Tetris/tetris.h"


namespace tetris {

void enter()
{
  initDisplay();
  initAudio();
  gameReset();
  drawPlayingUI();
  drawNextPiece();
  drawScore();
  isUp = false;
  isDown = false;
  isLeft = false;
  isRight = false;
  reseted = false;
}

void tick()
{
  if (!gameOverFlag)
  {
    erasedPlayingUI = false;
    erasedDeathScreen = false;
    if (reseted)
    {
      reseted = false;
    }
    movePiece();
    calculateGhostPiece();
    bool drawingNextPieceFlag = delayedFallAndLogic();
    renderBoard();
    if (drawingNextPieceFlag)
    {
      eraseNextPiece();
      drawNextPiece();
    }

    if (prevScore != score)
    {
      drawScore();
      prevScore = score;
    }
  }
  else
  {
    redrawUI = true;
    unsigned currGameOverTime = millis();
    if (currGameOverTime - lastGameOverTime > 1500)
    {
      if (!erasedDeathScreen)
      {
        eraseScreen();
        erasedDeathScreen = true;
      }
      drawGameOverUI();
      if (erasedDeathScreen && reseted)
      {
        gameReset();
        drawNextPiece();
        drawScore();
        drawPlayingUI();
        reseted = false;
      }
    }
    else
    {
      if (!erasedPlayingUI)
      {
        eraseScreen();
        erasedPlayingUI = true;
      }
      drawDeathUI();
    }
  }
}

void exit()
{
  shutdownAudio();
  tetrisCanvas.deleteSprite();
  tft.fillScreen(TFT_BLACK);
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
    case console::Button::Left:
      if (!gameOverFlag)
      {
        isLeft = true;
      }
      break;
    case console::Button::Right:
      if (!gameOverFlag)
      {
        isRight = true;
      }
      break;
    case console::Button::Reset:
      reseted = true;
      break;
  }
}

}
