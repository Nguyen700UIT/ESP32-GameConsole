#pragma once
#include <Arduino.h>

//SCREEN DEFINITIONS
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GAME_HEIGHT 200 // The playable area height. Bottom 40px is for UI (Score)

//PIN DEFINITIONS
#define UP 25   
#define RESET 13
#define BUZZER 32
#define BUZZER_SFX 21
#define POT 34
//GAME CONFIG
#define DEBOUNCE 150 //(Chống dội nút)

namespace flappy_bird {

extern unsigned long long lastTime;
extern volatile unsigned long lastInterruptTime;
extern volatile unsigned long lastResetInterruptTime;
extern volatile bool isUp;
extern volatile bool reseted;
extern int score;
extern int bestScore;
extern volatile bool gameOver;
extern bool lost;
extern QueueHandle_t sfxQueue;

enum sfxEvent {SOUND_JUMP, SOUND_SCORE};

void queueSend(sfxEvent sfx);

}
