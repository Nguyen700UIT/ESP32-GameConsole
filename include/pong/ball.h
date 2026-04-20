#pragma once
#include <Arduino.h>
#include "pong/display.h"
#include "pong/config.h"
#include <stdint.h>

#define BALL_SIZE 8
#define BALL_MAX_SPEED 10.0

namespace pong {

extern float ballX;
extern float ballY;
extern float ballSpeedX;
extern float ballSpeedY;

void resetBall();

}
