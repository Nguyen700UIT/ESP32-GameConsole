#include "flappyBird/config.h"

namespace flappy_bird {

unsigned long long lastTime = 0;
volatile unsigned long lastInterruptTime = 0;
volatile unsigned long lastResetInterruptTime = 0;
unsigned long timerNoWifi = 0;
unsigned long timeWithoutWifi = 0;
volatile bool isUp = false;
volatile bool reseted = false;
int score = 0;
int bestScore = 0;
volatile bool gameOver = false;
bool lost = false;
QueueHandle_t sfxQueue = NULL;

void queueSend(sfxEvent sfx)
{
    if (sfxQueue == NULL)
    {
        return;
    }

    if (xQueueSend(sfxQueue, &sfx, 0) != pdPASS)
    {
        return;
    }
}

}
