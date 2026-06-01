#pragma once
#include <Arduino.h>
#include "console/fps_counter.h"
#include "Snake/snake_bait.h"

namespace snake {

//Game 
extern const int BASE_MOVE_DELAY;
extern int moveDelay;
extern unsigned long prevTime;
extern unsigned long currTime;

extern int prevScore;
extern int score;
extern bool drewGameOverUI;
extern int bestScore;

enum BaitType
{
    BAIT_RED,
    BAIT_YELLOW,
    BAIT_GREEN,
    BAIT_BLUE,
    BAIT_RAINBOW
};

struct Bait
{
    int x;
    int y;
    uint16_t baitColor;
    BaitType type;
    bool active;
    unsigned long spawnedAt;
};

enum snakeColor
{
    RED, BLUE, YELLOW
};

extern Bait bait;
extern Bait specialBait;
extern int normalBaitEaten;
extern const uint16_t baitBmp[] PROGMEM;
extern const uint16_t baitBlueBmp[] PROGMEM;
extern const uint16_t baitYellowBmp[] PROGMEM;
extern const uint16_t baitGreenBmp[] PROGMEM;
extern const uint16_t baitRainbowBmp[] PROGMEM;

extern const uint16_t headBlueUp[] PROGMEM;
extern const uint16_t headBlueDown[] PROGMEM;
extern const uint16_t headBlueLeft[] PROGMEM;
extern const uint16_t headBlueRight[] PROGMEM;
extern const uint16_t snakeBlueBody[] PROGMEM;
extern const uint16_t headRedUp[] PROGMEM;
extern const uint16_t headRedDown[] PROGMEM;   
extern const uint16_t headRedLeft[] PROGMEM;
extern const uint16_t headRedRight[] PROGMEM;
extern const uint16_t snakeRedBody[] PROGMEM;
extern const uint16_t headYellowUp[] PROGMEM;
extern const uint16_t headYellowDown[] PROGMEM;
extern const uint16_t headYellowLeft[] PROGMEM;
extern const uint16_t headYellowRight[] PROGMEM;
extern const uint16_t snakeYellowBody[] PROGMEM;


snakeColor getColor();
void initBait();
void updateSpecialBait();
void readDirection();
void move();
bool eat();
void drawSnake(snakeColor color);
void eraseSnakeTail();
void drawBait(const Bait& currentBait);
void drawBait();
void isGameOver();
void drawPlayingUI();
void drawScore();
void drawFpsStats(const console::FpsStats& stats);
void drawGameOverUI();
void renderBoard(bool& ate, snakeColor color);
void gameReset();

}
