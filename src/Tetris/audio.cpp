#include "Tetris/audio.h"

namespace tetris {

u_int16_t tempo = 180;
TaskHandle_t musicTask = NULL;

const int melody[] PROGMEM = {
  NOTE_E5, 4, NOTE_B4,8, NOTE_C5,8, NOTE_D5,4, NOTE_C5,8, NOTE_B4,8,
  NOTE_A4, 4, NOTE_A4,8, NOTE_C5,8, NOTE_E5,4, NOTE_D5,8, NOTE_C5,8,
  NOTE_B4, -4, NOTE_C5,8, NOTE_D5,4, NOTE_E5,4,
  NOTE_C5, 4, NOTE_A4,4, NOTE_A4,8, NOTE_A4,4, NOTE_B4,8, NOTE_C5,8,
  NOTE_D5, -4, NOTE_F5,8, NOTE_A5,4, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5, -4, NOTE_C5,8, NOTE_E5,4, NOTE_D5,8, NOTE_C5,8,
  NOTE_B4, 4, NOTE_B4,8, NOTE_C5,8, NOTE_D5,4, NOTE_E5,4,
  NOTE_C5, 4, NOTE_A4,4, NOTE_A4,4, REST, 4,
  NOTE_E5, 4, NOTE_B4,8, NOTE_C5,8, NOTE_D5,4, NOTE_C5,8, NOTE_B4,8,
  NOTE_A4, 4, NOTE_A4,8, NOTE_C5,8, NOTE_E5,4, NOTE_D5,8, NOTE_C5,8,
  NOTE_B4, -4, NOTE_C5,8, NOTE_D5,4, NOTE_E5,4,
  NOTE_C5, 4, NOTE_A4,4, NOTE_A4,8, NOTE_A4,4, NOTE_B4,8, NOTE_C5,8,
  NOTE_D5, -4, NOTE_F5,8, NOTE_A5,4, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5, -4, NOTE_C5,8, NOTE_E5,4, NOTE_D5,8, NOTE_C5,8,
  NOTE_B4, 4, NOTE_B4,8, NOTE_C5,8, NOTE_D5,4, NOTE_E5,4,
  NOTE_C5, 4, NOTE_A4,4, NOTE_A4,4, REST, 4,
  NOTE_E5,2, NOTE_C5,2,
  NOTE_D5,2, NOTE_B4,2,
  NOTE_C5,2, NOTE_A4,2,
  NOTE_GS4,2, NOTE_B4,4, REST,8,
  NOTE_E5,2, NOTE_C5,2,
  NOTE_D5,2, NOTE_B4,2,
  NOTE_C5,4, NOTE_E5,4, NOTE_A5,2,
  NOTE_GS5,2,
};

void playMusic()
{
    int wholeNote = (60000 * 4) / tempo;
    int noteAmount = sizeof(melody) / sizeof(melody[0]) / 2;

    for (int currNode = 0; currNode < noteAmount * 2; currNode += 2)
    {
        if (gameOverFlag)
        {
            ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
            return;
        }

        int divider = melody[currNode + 1];
        int noteDuration = 0;
        if (divider > 0) noteDuration = wholeNote / divider;
        else if (divider < 0) noteDuration = (wholeNote / abs(divider)) * 1.5;

        ledcWriteTone(PWM_CHANNEL_BUZZER, melody[currNode]);
        for (int d = 0; d <= 60; d += 5)
        {
            ledcWrite(PWM_CHANNEL_BUZZER, d);
            vTaskDelay(5 / portTICK_PERIOD_MS);
        }

        vTaskDelay((noteDuration * 0.9) / portTICK_PERIOD_MS);

        for (int d = 60; d >= 0; d -= 5)
        {
            ledcWrite(PWM_CHANNEL_BUZZER, d);
            vTaskDelay(5 / portTICK_PERIOD_MS);
        }

        ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
        vTaskDelay((noteDuration * 0.1) / portTICK_PERIOD_MS);
    }
}

void audioTask(void *pvParameters)
{
    ledcSetup(PWM_CHANNEL_BUZZER, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(BUZZER, PWM_CHANNEL_BUZZER);

    while (true)
    {
        if (!gameOverFlag)
        {
            playMusic();
        }
        else
        {
            ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }
}

void initAudio()
{
    xTaskCreatePinnedToCore(audioTask, "MusicTask", 2048, NULL, 2, &musicTask, 0);
}

void shutdownAudio()
{
    if (musicTask != NULL)
    {
        vTaskDelete(musicTask);
        musicTask = NULL;
    }

    ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
}

}
