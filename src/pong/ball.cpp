#include "pong/ball.h"

namespace pong {

float ballX = 0;
float ballY = 0;
float ballSpeedX = 6.0;
float ballSpeedY = 6.0;

void resetBall()
{
    ballX = (SCREEN_WIDTH - BALL_SIZE) / 2;
    ballY = (GAME_HEIGHT - BALL_SIZE) / 2;
    ballSpeedX = -ballSpeedX;
}

}
