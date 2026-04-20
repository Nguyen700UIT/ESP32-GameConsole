#pragma once

//Screen definitions
#define SCREEN_WIDTH tft.width()
#define SCREEN_HEIGHT tft.height()

//Buttons
#define UP 25
#define DOWN 26  
#define LEFT 27
#define RIGHT 33
#define RESET 13
#define BUZZER 32
#define BUTTON_DEBOUNCE 100
//Game definitions
#define BLOCK_SIZE 20
#define GRAVITY BLOCK_SIZE
#define TETRIS_BOARD_WIDTH 64
#define TETRIS_BOARD_HEIGHT 64
#define UI_BOARD_WIDTH 64
#define UI_BOARD_HEIGHT 64



namespace tetris {

extern volatile bool isUp;
extern volatile bool isDown;
extern volatile bool isLeft;
extern volatile bool isRight;
extern volatile bool reseted;
extern volatile unsigned long lastInterruptTime[4];
extern volatile unsigned long lastResetInterruptTime;
extern volatile bool gameOverFlag;

}
