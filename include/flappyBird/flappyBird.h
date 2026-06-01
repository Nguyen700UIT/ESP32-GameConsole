#pragma once
#include "flappyBird/display.h"
#include "flappyBird/bird.h"
#include "flappyBird/tube.h"
#include "console/fps_counter.h"

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
void drawFpsStats(const console::FpsStats& stats);
void renderGame(timeTheme theme);
void handleGameOver();

}
