#pragma once
#include <Arduino.h>

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
#define BUZZER 32
#define BUZZER_SFX 21
#define POT 34

namespace pong {

extern volatile bool isUp;
extern volatile bool isDown;
extern volatile bool reseted;
extern volatile unsigned long lastInterruptTime[2];
extern volatile unsigned long lastResetInterruptTime;
extern volatile bool gameOverFlag;

extern QueueHandle_t sfxQueue;
enum sfxEvent {SOUND_HIT, SOUND_SCORE}; // Đổi tên sự kiện cho phù hợp với Snake

void queueSend(sfxEvent sfx);
}
