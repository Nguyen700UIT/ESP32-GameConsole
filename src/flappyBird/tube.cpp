#include "flappyBird/tube.h"

namespace flappy_bird {

int tubeGap = 70;
int TubeWidth = 26;
int capWidth = 32;
int capHeight = 12;
int capOffsetX = 3;
Tube tube;
uint16_t TUBE_GREEN = tft.color565(3, 249, 67);

Tube::Tube()
{
  tubeX[0] = 320;
  tubeX[1] = 470;
  tubeX[2] = 620;
}

void initTube()
{
  for (int i = 0; i < 3; ++i) {
    tube.upperTubeHeight[i] = random(20, GAME_HEIGHT - tubeGap - 20);
    tube.lowerTubeHeight[i] = GAME_HEIGHT - tube.upperTubeHeight[i] - tubeGap;
    tube.passed[i] = false;
  }
}

void drawTubeBody(int x, int y, int h)
{
    canvas.fillRect(x, y, 2, h, TFT_BLACK);
    canvas.fillRect(x + 2, y, 4, h, TFT_WHITE);
    canvas.fillRect(x + 6, y, 18, h, TUBE_GREEN);
    canvas.fillRect(x + 24, y, 2, h, TFT_BLACK);
}

void drawTubeCap(int bodyX, int y)
{
    int capX = bodyX - capOffsetX;
    canvas.fillRect(capX, y, capWidth, capHeight, TFT_BLACK);
    canvas.fillRect(capX + 2, y + 2, capWidth - 4, capHeight - 4, TUBE_GREEN);
    canvas.fillRect(bodyX + 2, y + 2, 4, capHeight - 4, TFT_WHITE);
}

void drawTube()
{
  for (int i = 0; i < 3; ++i) {
    drawTubeBody(tube.tubeX[i], 0, tube.upperTubeHeight[i]);
    drawTubeCap(tube.tubeX[i], tube.upperTubeHeight[i] - capHeight);

    int lowerTubeY = tube.upperTubeHeight[i] + tubeGap;
    drawTubeBody(tube.tubeX[i], lowerTubeY, GAME_HEIGHT - lowerTubeY);
    drawTubeCap(tube.tubeX[i], lowerTubeY);
  }
}

void updateTubes()
{
  for (int i = 0; i < 3; ++i) {
    tube.tubeX[i] -= 2;

    if (tube.tubeX[i] + capWidth < 0) {
      int maxX = tube.tubeX[0];
      for (int j = 0; j < 3; ++j) {
        maxX = max(maxX, tube.tubeX[j]);
      }

      tube.tubeX[i] = maxX + 150;
      tube.upperTubeHeight[i] = random(20, GAME_HEIGHT - tubeGap - 20);
      tube.lowerTubeHeight[i] = GAME_HEIGHT - tube.upperTubeHeight[i] - tubeGap;
      tube.passed[i] = false;
    }
  }
}

}
