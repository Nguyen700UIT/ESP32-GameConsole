#pragma once
#include "pong/config.h"
#include "pong/display.h"
#include "pong/paddle.h"
#include "pong/ball.h"
#include "console/fps_counter.h"


#define AI_PREDICTION_INTERVAL 100
#define LOG_INTERVAL 20
namespace pong {

extern int playerScore;
extern int aiScore;
extern bool lost;
extern int aiSpeed;
extern int aiReaction;
extern unsigned long lastPredictionTime;
extern unsigned long lastLogTime;

void gameLogicForHumanML();
void gameLogicForAIML();
void aiMoveUsingML();
int aiMoveForML();
int playerMoveForML();
void logPlayer(int action);
void logAI(int action);
void playerMove();
void aiMove();
void ballBehavior();
void scoring();
void gameLogic();
void resetGame();
void drawPlayingUI();
void drawScore();
void drawFpsStats(const console::FpsStats& stats);
void render();
void drawGameOver();
}
