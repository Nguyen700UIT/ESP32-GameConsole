#pragma once
#include "flappyBird/config.h"
#include "flappyBird/display.h"
#include "flappyBird/bird.h"
#include "flappyBird/tube.h"

namespace flappy_bird {

enum timeTheme
{
    DAY, NIGHT
};



timeTheme getTheme();
void collisionLogic();
void scored();
void resetGame();
void renderGame(timeTheme theme);
void handleGameOver();

}
