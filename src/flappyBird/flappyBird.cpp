#include "flappyBird/flappyBird.h"
#include "flappyBird/background.h"
#include "flappyBird/wifi_time.h"

namespace flappy_bird {

namespace {
constexpr uint16_t BACKGROUND_TRANSPARENT = 0x0000;
constexpr int CELESTIAL_X = 36;
constexpr int CELESTIAL_Y = 25;
int prevScore = -1;
int prevBestScore = -1;
}

timeTheme getTheme()
{
  struct tm timeinfo;
  if (!syncTime(timeinfo)) 
  {
    return MORNING;
  }  
  int hour = timeinfo.tm_hour;

  if (hour >= 6 && hour < 14) return MORNING;
  if (hour >= 17 || hour < 6) return NIGHT;
  return NOON;

}

void drawMorning()
{
  canvas.fillSprite(TFT_SKYBLUE);
  drawBitmapTransparent(canvas, CELESTIAL_X, CELESTIAL_Y, SUN_WIDTH, SUN_HEIGHT, sunMorning, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 62, 22, CLOUD_WIDTH, CLOUD_HEIGHT, cloudMorning1, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 142, 48, CLOUD_WIDTH, CLOUD_HEIGHT, cloudMorning2, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 218, 28, CLOUD_WIDTH, CLOUD_HEIGHT, cloudMorning1, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 274, 78, CLOUD_WIDTH, CLOUD_HEIGHT, cloudMorning2, BACKGROUND_TRANSPARENT);
  //
  drawBitmapTransparent(canvas, 90, 85, CLOUD_WIDTH, CLOUD_HEIGHT, cloudMorning2, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 175, 95, CLOUD_WIDTH, CLOUD_HEIGHT, cloudMorning1, BACKGROUND_TRANSPARENT);
  //
  drawBitmapTransparent(canvas, 40, 130, CLOUD_WIDTH, CLOUD_HEIGHT, cloudMorning1, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 150, 145, CLOUD_WIDTH, CLOUD_HEIGHT, cloudMorning1, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 240, 130, CLOUD_WIDTH, CLOUD_HEIGHT, cloudMorning2, BACKGROUND_TRANSPARENT);
}

void drawNoon()
{
  uint16_t colorNoon = tft.color565(253, 179, 115);
  canvas.fillSprite(colorNoon);
  drawBitmapTransparent(canvas, CELESTIAL_X, CELESTIAL_Y, SUN_WIDTH, SUN_HEIGHT, sunNoon, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 52, 28, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNoon1, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 118, 66, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNoon2, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 198, 34, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNoon1, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 274, 84, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNoon2, BACKGROUND_TRANSPARENT);
  //
  drawBitmapTransparent(canvas, 30, 100, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNoon2, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 170, 115, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNoon1, BACKGROUND_TRANSPARENT);
  //
  drawBitmapTransparent(canvas, 60, 150, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNoon1, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 230, 160, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNoon2, BACKGROUND_TRANSPARENT);
}

void drawNight()
{
  uint16_t colorNight = tft.color565(66, 67, 113);
  canvas.fillSprite(colorNight);
  drawBitmapTransparent(canvas, CELESTIAL_X, CELESTIAL_Y, MOON_WIDTH, MOON_HEIGHT, moon, BACKGROUND_TRANSPARENT);

  const unsigned long blinkFrame = (millis() / 500) % 8;
  if (blinkFrame != 1) {
    drawBitmapTransparent(canvas, 58, 24, STAR_WIDTH, STAR_HEIGHT, star, BACKGROUND_TRANSPARENT);
  }
  if (blinkFrame != 2) {
    drawBitmapTransparent(canvas, 138, 50, STAR_WIDTH, STAR_HEIGHT, star, BACKGROUND_TRANSPARENT);
  }
  if (blinkFrame != 3) {
    drawBitmapTransparent(canvas, 250, 34, STAR_WIDTH, STAR_HEIGHT, star, BACKGROUND_TRANSPARENT);
  }
  if (blinkFrame != 4) {
    drawBitmapTransparent(canvas, 35, 125, STAR_WIDTH, STAR_HEIGHT, star, BACKGROUND_TRANSPARENT);
  }
  if (blinkFrame != 5) {
    drawBitmapTransparent(canvas, 182, 92, STAR_WIDTH, STAR_HEIGHT, star, BACKGROUND_TRANSPARENT);
  }
  if (blinkFrame != 6) {
    drawBitmapTransparent(canvas, 285, 160, STAR_WIDTH, STAR_HEIGHT, star, BACKGROUND_TRANSPARENT);
  }
  if (blinkFrame != 7) {
    drawBitmapTransparent(canvas, 100, 152, STAR_WIDTH, STAR_HEIGHT, star, BACKGROUND_TRANSPARENT);
  }

  drawBitmapTransparent(canvas, 165, 30, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNight2, BACKGROUND_TRANSPARENT);
  //
  drawBitmapTransparent(canvas, 54, 64, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNight1, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 122, 92, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNight2, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 220, 58, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNight1, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 272, 100, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNight2, BACKGROUND_TRANSPARENT);
  //
  drawBitmapTransparent(canvas, 20, 145, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNight2, BACKGROUND_TRANSPARENT);
  drawBitmapTransparent(canvas, 190, 120, CLOUD_WIDTH, CLOUD_HEIGHT, cloudNight1, BACKGROUND_TRANSPARENT);
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
  drawPlayingUI();
}

void drawPlayingUI()
{
  tft.drawFastHLine(0, GAME_HEIGHT, SCREEN_WIDTH, TFT_WHITE);
  tft.fillRect(0, GAME_HEIGHT + 1, SCREEN_WIDTH, SCREEN_HEIGHT - GAME_HEIGHT, TFT_DARKGREY);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setCursor(10, GAME_HEIGHT + 12);
  tft.print("SCORE: ");
  tft.setCursor(180, GAME_HEIGHT + 12);
  tft.print("BEST: ");

  prevScore = -1;
  prevBestScore = -1;
  drawScore();
}

void drawScore()
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);

  if (score != prevScore)
  {
    tft.fillRect(85, GAME_HEIGHT + 12, 70, 16, TFT_DARKGREY);
    tft.setCursor(85, GAME_HEIGHT + 12);
    tft.printf("%05d", score);
    prevScore = score;
  }

  if (bestScore != prevBestScore)
  {
    tft.fillRect(245, GAME_HEIGHT + 12, 70, 16, TFT_DARKGREY);
    tft.setCursor(245, GAME_HEIGHT + 12);
    tft.printf("%05d", bestScore);
    prevBestScore = bestScore;
  }
}

void renderGame(timeTheme theme)
{
  if (wifiConnected) //If wifi connected use timeTheme
    switch(theme)
    {
      case MORNING:
        drawMorning();
        break;
      case NOON:
        drawNoon();
        break;
      case NIGHT:
        drawNight();
        break;
    }
  //Case when wifi not connected
  else
  {
    if (millis() - timerNoWifi >= 1000)
    {
      timerNoWifi = millis();
      timeWithoutWifi++;
    }
    int decision = (timeWithoutWifi / 60) % 3;
    switch(decision)
    {
      case 0:
        drawMorning();
        break;
      case 1:
        drawNoon();
        break;
      case 2:
        drawNight();
        break;
    }
  }


  drawTube();
  updateAnimation();
  drawBird();
  canvas.pushSprite(0, 0);
  drawScore();
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
