#pragma once
#include "flappyBird/config.h"
#include "flappyBird/display.h"
#include "flappyBird/bird.h"
#include "flappyBird/tube.h"

namespace flappy_bird {




timeTheme getTheme();
void drawMorning();
void drawNoon();
void drawNight();
void collisionLogic();
void scored();
void resetGame();
void drawPlayingUI();
void drawScore();
void renderGame(timeTheme theme);
void handleGameOver();

}
