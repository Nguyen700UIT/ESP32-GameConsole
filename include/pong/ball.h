#pragma once
#include <Arduino.h>
#include "pong/display.h"
#include "pong/config.h"

#define BALL_SIZE 8
#define BALL_MAX_SPEED 6.0
#define BALL_BASE_SPEED 4.0
#define MIN_BALL_SPEED_X (BALL_BASE_SPEED / 2.0)

namespace pong {

extern float ballX;
extern float ballY;
extern float ballSpeedX;
extern float ballSpeedY;

void resetBall();

}
