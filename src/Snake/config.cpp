#include "Snake/config.h"

namespace snake {

bool gameOver = false;
volatile bool isUp = false;
volatile bool isDown = false;
volatile bool isLeft = false;
volatile bool isRight = false;
volatile bool reseted = false;
volatile unsigned long lastInterruptTime[4] = {0, 0, 0, 0};
volatile unsigned long lastResetInterruptTime = 0;

QueueHandle_t sfxQueue = NULL;

void queueSend(sfxEvent sfx)
{
    {
        if (sfxQueue == NULL) return;
        if (xQueueSend(sfxQueue, &sfx, 0) != pdPASS) return;
    }
}

}
