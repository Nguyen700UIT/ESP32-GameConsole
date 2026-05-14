#include "flappyBird/audio.h"

namespace flappy_bird {

u_int16_t tempoTheme = 150;
u_int16_t tempoGameOver = 150;
u_int16_t volume = 0;
TaskHandle_t musicTask = NULL;
TaskHandle_t SFXTask = NULL;

const int melodyTheme[] PROGMEM = {
  // Measure 3
  NOTE_DS5, -4, NOTE_FS5, 8, NOTE_F5, 8, NOTE_FS5, 8, NOTE_AS4, 8, NOTE_CS5, 8,
  
  // Measure 4
  NOTE_DS5, 4, NOTE_CS5, 8, NOTE_DS5, 8, NOTE_AS4, 8, NOTE_AS4, 16, NOTE_GS4, 16, NOTE_AS4, 8, NOTE_CS5, 8,
  
  // Measure 5
  NOTE_DS5, -4, NOTE_FS5, 8, NOTE_F5, 8, NOTE_FS5, 8, NOTE_GS5, 8, NOTE_AS5, 8,
  
  // Measure 6
  NOTE_DS5, 4, NOTE_CS5, 8, NOTE_DS5, 8, NOTE_F5, 8, NOTE_FS5, 16, NOTE_F5, 16, NOTE_DS5, 4,
  
  // Measure 7
  NOTE_GS5, 4, NOTE_GS5, 8, NOTE_AS5, 8, NOTE_FS5, 4, NOTE_DS5, 8, NOTE_FS5, 8,
  
  // Measure 8
  NOTE_GS5, 8, REST, 8, NOTE_AS5, 8, NOTE_FS5, 4, NOTE_DS5, 8, NOTE_FS5, 8,
  
  // Measure 9
  NOTE_F5, 8, REST, 4, NOTE_F5, 16, REST, 16, NOTE_AS5, 16, NOTE_AS5, 16, NOTE_GS5, 16, REST, 16, NOTE_AS4, 8,
  
  // Measure 10
  NOTE_CS5, 8, NOTE_DS5, 2, NOTE_DS5, 8, NOTE_DS4, 16, NOTE_DS4, 16, NOTE_CS4, 8
};

const int melodyGameOver[] PROGMEM = {
  NOTE_FS5, 8, NOTE_F5, 8, NOTE_DS5, 8, NOTE_CS5, 8, NOTE_DS5, 8, NOTE_AS4, 8,
  NOTE_C5, 4, NOTE_GS4, 4, NOTE_DS5, 8, NOTE_F5, 8,
  NOTE_FS5, 4, NOTE_GS5, 4, NOTE_CS6, 4,
  NOTE_AS5, -2,
  NOTE_FS5, 8, NOTE_F5, 8, NOTE_DS5, 8, NOTE_CS5, 8, NOTE_DS5, 8, NOTE_AS4, 8,
  NOTE_C5, 4, NOTE_GS4, 4, NOTE_DS4, 8, NOTE_F4, 8,
  NOTE_FS4, 4, NOTE_F4, 4, NOTE_CS4, 4,
  NOTE_DS4, -2, NOTE_DS4, -2,
  NOTE_GS5, 8, NOTE_FS5, 8, NOTE_E5, 8, NOTE_DS5, 8, NOTE_CS5, 8, NOTE_E5, 8,
  NOTE_DS5, 4, NOTE_AS4, 4, NOTE_AS4, 8, NOTE_DS5, 8,
  NOTE_GS5, 8, NOTE_FS5, 8, NOTE_E5, 8, NOTE_DS5, 8, NOTE_CS5, 8, NOTE_E5, 8,
  NOTE_DS5, 2, NOTE_DS4, 8, NOTE_GS4, 8,
  NOTE_CS5, 8, NOTE_C5, 8, NOTE_AS4, 8, NOTE_GS4, 8, NOTE_AS4, 8, NOTE_C5, 8,
  NOTE_AS4, 4, NOTE_DS4, 4, NOTE_DS4, 8, NOTE_F4, 8,
  NOTE_FS4, 4, NOTE_B4, 4, NOTE_DS5, 4,
  NOTE_D5, -2,
  NOTE_CS5, 8, NOTE_C5, 8, NOTE_AS4, 8, NOTE_GS4, 8, NOTE_AS4, 8, NOTE_C5, 8,
  NOTE_AS4, 4, NOTE_DS4, 4, NOTE_DS4, 8, NOTE_F4, 8,
  NOTE_FS4, 4, NOTE_F4, 4, NOTE_CS4, 4,
  NOTE_DS4, -2
};

u_int16_t smoothRead(const int& PIN)
{
    int sum = 0;
    for (int i = 0; i < 10; ++i)
    {
        sum += analogRead(PIN);
    }
    u_int16_t finalVolume = (u_int16_t)(sum / 10);
    finalVolume = constrain(finalVolume, 0, 40);
    return finalVolume;
}

void playRickRoll()
{
    int wholeNoteTheme = (60000 * 4) / tempoTheme;
    int noteAmountTheme = sizeof(melodyTheme) / sizeof(melodyTheme[0]) / 2;

    for (int currNote = 0; currNote < noteAmountTheme * 2; currNote += 2)
    {
        if (gameOver) return;
        int divider = melodyTheme[currNote + 1];
        int noteDuration = 0;
        if (divider > 0) noteDuration = wholeNoteTheme / divider;
        else if (divider < 0) noteDuration = (wholeNoteTheme / abs(divider)) * 1.5;

        volume = smoothRead(POT);
        ledcWriteTone(PWM_CHANNEL_BUZZER, melodyTheme[currNote]);
        ledcWrite(PWM_CHANNEL_BUZZER, volume);
        vTaskDelay((noteDuration * 0.9) / portTICK_PERIOD_MS);
        ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
        vTaskDelay((noteDuration * 0.1) / portTICK_PERIOD_MS);
    }
}

void playGameOverMusic()
{
    int wholeNoteOver = (60000 * 4) / tempoGameOver;
    int noteAmountOver = sizeof(melodyGameOver) / sizeof(melodyGameOver[0]) / 2;

    for (int currNote = 0; currNote < noteAmountOver * 2; currNote += 2)
    {
        if (!gameOver) return;
        int divider = melodyGameOver[currNote + 1];
        int noteDuration = 0;
        if (divider > 0) noteDuration = wholeNoteOver / divider;
        else if (divider < 0) noteDuration = (wholeNoteOver / abs(divider)) * 1.5;

        volume = smoothRead(POT);
        ledcWriteTone(PWM_CHANNEL_BUZZER, melodyGameOver[currNote]);
        ledcWrite(PWM_CHANNEL_BUZZER, volume);
        vTaskDelay((noteDuration * 0.9) / portTICK_PERIOD_MS);
        ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
        vTaskDelay((noteDuration * 0.1) / portTICK_PERIOD_MS);
    }
}

void playDieSFX()
{
    volume = smoothRead(POT);
    ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
    for (int freq = 1000; freq > 200; freq -= 100)
    {
        ledcWriteTone(PWM_CHANNEL_BUZZER, freq);
        ledcWrite(PWM_CHANNEL_BUZZER, volume);
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }

    ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
}

void playJumpSFX()
{
    volume = smoothRead(POT);
    ledcWriteTone(PWM_CHANNEL_BUZZER_SFX, NOTE_FS6);
    ledcWrite(PWM_CHANNEL_BUZZER_SFX, volume);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    ledcWriteTone(PWM_CHANNEL_BUZZER_SFX, 0);
}

void playScoreSFX()
{
    volume = smoothRead(POT);
    ledcWrite(PWM_CHANNEL_BUZZER_SFX, volume);
    ledcWriteTone(PWM_CHANNEL_BUZZER_SFX, NOTE_C7);
    vTaskDelay(40 / portTICK_PERIOD_MS);
    ledcWriteTone(PWM_CHANNEL_BUZZER_SFX, NOTE_E7);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    ledcWriteTone(PWM_CHANNEL_BUZZER_SFX, 0);
}

void audioMusicTask(void *pvParameters)
{
    ledcSetup(PWM_CHANNEL_BUZZER, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(BUZZER, PWM_CHANNEL_BUZZER);
    bool playedSFXOnce = false;

    while (true)
    {
        if (!gameOver)
        {
            playedSFXOnce = false;
            playRickRoll();
        }
        else
        {
            if (!playedSFXOnce)
            {
                playDieSFX();
                playedSFXOnce = true;
                if (sfxQueue != NULL)
                {
                    xQueueReset(sfxQueue);
                }
            }

            playGameOverMusic();
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void audioSFXTask(void *pvParameters)
{
    ledcSetup(PWM_CHANNEL_BUZZER_SFX, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(BUZZER_SFX, PWM_CHANNEL_BUZZER_SFX);
    sfxEvent eventHappening;

    while (true)
    {
        if (xQueueReceive(sfxQueue, &eventHappening, portMAX_DELAY) == pdPASS)
        {
            switch (eventHappening)
            {
                case SOUND_JUMP:
                    playJumpSFX();
                    break;
                case SOUND_SCORE:
                    playScoreSFX();
                    break;
            }
        }
    }
}

void initAudio()
{
    sfxQueue = xQueueCreate(10, sizeof(sfxEvent));
    analogReadResolution(PWM_RESOLUTION - 1);
    xTaskCreatePinnedToCore(audioMusicTask, "MusicTask", 4096, NULL, 2, &musicTask, 0);
    xTaskCreatePinnedToCore(audioSFXTask, "SFXTask", 2048, NULL, 2, &SFXTask, 0);
}

void shutdownAudio()
{
    if (musicTask != NULL)
    {
        vTaskDelete(musicTask);
        musicTask = NULL;
    }

    if (SFXTask != NULL)
    {
        vTaskDelete(SFXTask);
        SFXTask = NULL;
    }

    if (sfxQueue != NULL)
    {
        vQueueDelete(sfxQueue);
        sfxQueue = NULL;
    }

    ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
    ledcWriteTone(PWM_CHANNEL_BUZZER_SFX, 0);
}

}
