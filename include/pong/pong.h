#pragma once
#include "pong/config.h"
#include "pong/display.h"
#include "pong/paddle.h"
#include "pong/ball.h"

#define AI_PREDICTION_INTERVAL 100

namespace pong {

extern int playerScore;
extern int aiScore;
extern bool lost;
extern int aiSpeed;
extern int aiReaction;
extern unsigned long lastPredictionTime;


void playerMove();
void aiMove();
void ballBehavior();
void scoring();
void gameLogic();
void resetGame();
void render();
void drawGameOver();

}
