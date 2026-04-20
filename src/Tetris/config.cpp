#include "Tetris/config.h"

namespace tetris {

volatile bool gameOverFlag = false;
volatile bool isUp = false;
volatile bool isDown = false;
volatile bool isLeft = false;
volatile bool isRight = false;
volatile bool reseted = false;
volatile unsigned long lastInterruptTime[4] = {0};
volatile unsigned long lastResetInterruptTime = 0;

}
