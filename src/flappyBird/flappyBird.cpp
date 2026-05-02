#include "flappyBird/flappyBird.h"

namespace flappy_bird {

void collisionLogic()
{
  for (int i = 0; i < 3; ++i) {
    if (BIRD_X + birdWidth > tube.tubeX[i] && tube.tubeX[i] + TubeWidth > BIRD_X) {
      int top = tube.upperTubeHeight[i];
      int bot = tube.upperTubeHeight[i] + tubeGap;

      if (birdY < top || birdY + birdHeight > bot) {
        gameOver = true;
      }
    }
  }

  if (birdY <= 0 || birdY + birdHeight >= GAME_HEIGHT) {
    gameOver = true;
  }
}

void scored()
{
  for (int i = 0; i < 3; ++i) {
    if (BIRD_X >= tube.tubeX[i] + TubeWidth && tube.passed[i] == false) {
      score++;
      tube.passed[i] = true;
      bestScore = max(bestScore, score);
      queueSend(SOUND_SCORE);
    }
  }
}

void resetGame()
{
  gameOver = false;
  score = 0;
  tube = Tube();
  initTube();
  resetBird();
  lost = false;
  isUp = false;
}

void renderGame()
{
  canvas.fillSprite(TFT_SKYBLUE);
  drawTube();
  updateAnimation();
  drawBird();
  canvas.drawFastHLine(0, GAME_HEIGHT, SCREEN_WIDTH, TFT_WHITE);
  canvas.fillRect(0, GAME_HEIGHT + 1, SCREEN_WIDTH, SCREEN_HEIGHT - GAME_HEIGHT, TFT_DARKGREY);
  canvas.setTextSize(2);
  canvas.setTextColor(TFT_WHITE);
  canvas.setCursor(10, GAME_HEIGHT + 12);
  canvas.printf("SCORE: %d", score);
  canvas.setCursor(180, GAME_HEIGHT + 12);
  canvas.printf("BEST: %d", bestScore);
  canvas.pushSprite(0, 0);
}

void handleGameOver()
{
  if (lost == false) {
    lost = true;
    canvas.setTextSize(3);
    canvas.setTextColor(TFT_RED);
    canvas.setCursor(70, 80);
    canvas.println("GAME OVER");
    canvas.pushSprite(0, 0);
  }
}

}
