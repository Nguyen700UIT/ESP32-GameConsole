#pragma once
//Screen definitions
#define SCREEN_WIDTH tft.width()
#define SCREEN_HEIGHT tft.height()
#define GAME_HEIGHT 200
//Buttons
#define UP 25
#define DOWN 26  
#define LEFT 27
#define RIGHT 33
#define RESET 13
#define BUZZER 32
#define BUTTON_DEBOUNCE 100

namespace pong {

extern volatile bool isUp;
extern volatile bool isDown;
extern volatile bool reseted;
extern volatile unsigned long lastInterruptTime[2];
extern volatile unsigned long lastResetInterruptTime;
extern volatile bool gameOverFlag;

}
