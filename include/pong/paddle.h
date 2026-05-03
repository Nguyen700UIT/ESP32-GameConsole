#pragma once
#include <Arduino.h>
#include "pong/display.h"
#include "pong/config.h"
#include <stdint.h>

#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 30
#define PADDLE_SPEED 6

namespace pong {

extern int paddleLeftY;    // Player paddle
extern int paddleRightY;   // AI paddle

}
