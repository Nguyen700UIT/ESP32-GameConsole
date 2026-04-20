#pragma once
#include "flappyBird/config.h"
#include "flappyBird/display.h"

namespace flappy_bird {

struct Tube {
  bool passed[3];           // True if bird has passed this tube (for scoring)
  int tubeX[3];             // Current X position of the tubes
  int upperTubeHeight[3];   // Height of the top tube
  int lowerTubeHeight[3];   // Height of the bottom tube
  Tube();                   // Constructor to initialize default values
};

extern int tubeGap;
extern int TubeWidth;
extern int capWidth;
extern int capHeight;
extern int capOffsetX;

extern Tube tube;
extern uint16_t TUBE_GREEN;

// Tube functions
void initTube();
void drawTubeBody(int x, int y, int h);
void drawTubeCap(int bodyX, int y);
void drawTube();
void updateTubes();

}
