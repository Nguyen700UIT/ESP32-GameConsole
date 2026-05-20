#include "flappyBird/flappyBird.h"
#include "flappyBird/wifi_time.h"

namespace flappy_bird {


timeTheme getTheme()
{
  if (!getTime()) return DAY;

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo, 100)) return DAY;


  int hour = timeinfo.tm_hour;

  if (hour >= 17 || hour < 6) return NIGHT;
  return DAY;

}

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

void renderGame(timeTheme theme)
{
  switch(theme)
  {
    case DAY:
      canvas.fillSprite(TFT_SKYBLUE);
      break;
    case NIGHT:
      canvas.fillSprite(TFT_BLACK);
      break;
  }

  drawTube();
  updateAnimation();
  drawBird();
  canvas.pushSprite(0, 0);

  tft.drawFastHLine(0, GAME_HEIGHT, SCREEN_WIDTH, TFT_WHITE);
  tft.fillRect(0, GAME_HEIGHT + 1, SCREEN_WIDTH, SCREEN_HEIGHT - GAME_HEIGHT, TFT_DARKGREY);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setCursor(10, GAME_HEIGHT + 12);
  tft.printf("SCORE: %d", score);
  tft.setCursor(180, GAME_HEIGHT + 12);
  tft.printf("BEST: %d", bestScore);
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
