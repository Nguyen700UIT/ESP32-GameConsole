#include "pong/ball.h"

namespace pong {

float ballX = 0;
float ballY = 0;
float ballSpeedX = BALL_BASE_SPEED;
float ballSpeedY = BALL_BASE_SPEED;

void resetBall()
{
    ballX = (SCREEN_WIDTH - BALL_SIZE) / 2;
    ballY = (GAME_HEIGHT - BALL_SIZE) / 2;
    ballSpeedX = -ballSpeedX;
}

}
