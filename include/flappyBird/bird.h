#pragma once
#include "flappyBird/config.h"
#include "flappyBird/display.h"

// Fixed X position of the bird (It only moves up and down)
#define BIRD_X 60

namespace flappy_bird {

extern int birdFrame;
extern float birdY;
extern int birdWidth;
extern int birdHeight;
extern float birdVelocity;
extern const float GRAVITY;
extern float jumpForce;

void updateAnimation();
void drawBird();
void birdDropLogic();
void birdJumpLogic();
void resetBird();

}
