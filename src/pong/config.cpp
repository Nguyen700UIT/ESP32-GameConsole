#include "pong/config.h"

namespace pong {

volatile bool gameOverFlag = false;
volatile bool isUp = false;
volatile bool isDown = false;
volatile bool reseted = false;
volatile unsigned long lastInterruptTime[2] = {0};
volatile unsigned long lastResetInterruptTime = 0;

}
