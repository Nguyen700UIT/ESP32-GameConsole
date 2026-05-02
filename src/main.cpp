#include <Arduino.h>
#include <WiFi.h>
#include <esp_bt.h>
#include <TFT_eSPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "apps/flappy_bird_app.h"
#include "apps/pong_app.h"
#include "apps/snake_app.h"
#include "apps/tetris_app.h"
#include "console/button.h"

namespace {
//Pin definitions
constexpr uint8_t kUpPin = 25;
constexpr uint8_t kDownPin = 26;
constexpr uint8_t kLeftPin = 27;
constexpr uint8_t kRightPin = 33;
constexpr uint8_t kResetPin = 13;
constexpr uint8_t kBL = 22;
constexpr uint8_t kButtonCount = 5;
constexpr unsigned long kDebounceMs = 10;
constexpr unsigned long kMenuHoldMs = 2000;
constexpr uint8_t kButtonQueueLength = 16;

struct ButtonEvent {
  uint8_t buttonIdx;
  bool pressed;
  unsigned long atMs;
};

struct GameEntry {
  const char* name;
  //Function pointers
  void (*enter)(); 
  void (*tick)();
  void (*exit)();
  void (*onPress)(console::Button button);
  void (*onRelease)(console::Button button);
};

const uint8_t kButtonPins[kButtonCount] = {
  kUpPin,
  kDownPin,
  kLeftPin,
  kRightPin,
  kResetPin,
};

const console::Button kButtons[kButtonCount] = {
  console::Button::Up,
  console::Button::Down,
  console::Button::Left,
  console::Button::Right,
  console::Button::Reset,
};

const GameEntry kGames[] = {
  {"Flappy Bird", flappy_bird::enter, flappy_bird::tick, flappy_bird::exit, flappy_bird::onButtonPressed, flappy_bird::onButtonReleased},
  {"Snake", snake::enter, snake::tick, snake::exit, snake::onButtonPressed, snake::onButtonReleased},
  {"Pong", pong::enter, pong::tick, pong::exit, pong::onButtonPressed, pong::onButtonReleased},
  {"Tetris", tetris::enter, tetris::tick, tetris::exit, tetris::onButtonPressed, tetris::onButtonReleased},
};

constexpr size_t kGameCount = sizeof(kGames) / sizeof(kGames[0]);

TFT_eSPI menuTft;
const GameEntry* activeGame = nullptr; //Con tro hang 
size_t selectedIndex = 0;
bool menuDirty = true;
volatile unsigned long isrLastChangeAt[kButtonCount] = {0};
bool stablePressed[kButtonCount] = {false};
unsigned long resetPressedAt = 0;
bool resetLongHandled = false;
QueueHandle_t buttonEventQueue = nullptr;


uint16_t rgb(uint8_t r, uint8_t g, uint8_t b)
{
  return menuTft.color565(r, g, b);
}

void IRAM_ATTR buttonISR(void *arg)
{
  uint32_t buttonIdx = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(arg));
  unsigned long now = xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;

  if(now - isrLastChangeAt[buttonIdx] >= kDebounceMs)
  {
    bool currentRaw = digitalRead(kButtonPins[buttonIdx]) == LOW;
    ButtonEvent event = {
      static_cast<uint8_t>(buttonIdx),
      currentRaw,
      now,
    };

    BaseType_t higherPriorityTaskWoken = pdFALSE;

    isrLastChangeAt[buttonIdx] = now;
    if (buttonEventQueue != nullptr)
    {
      xQueueSendFromISR(buttonEventQueue, &event, &higherPriorityTaskWoken);
      portYIELD_FROM_ISR(higherPriorityTaskWoken); //Run prioritized task when ISR end
    }
  }
}

void drawMenu()
{
  const uint16_t bg = rgb(10, 18, 28);
  const uint16_t panel = rgb(26, 44, 61);
  const uint16_t accent = rgb(255, 184, 76);
  const uint16_t soft = rgb(87, 131, 160);
  const uint16_t text = TFT_WHITE;

  menuTft.fillScreen(bg);
  menuTft.fillRoundRect(12, 12, 296, 42, 10, panel);
  menuTft.setTextColor(text, panel);
  menuTft.setTextSize(2);
  menuTft.setCursor(24, 22);
  menuTft.println("MAY CHOI GAME ESP32");
  menuTft.setTextSize(1);
  menuTft.setTextColor(soft, panel);
  menuTft.setCursor(24, 42);
  menuTft.println("UP/DOWN chon  RIGHT start GIU RESET ra menu");

  for (size_t i = 0; i < kGameCount; ++i)
  {
    const int y = 70 + static_cast<int>(i) * 38;
    const bool selected = (i == selectedIndex);
    const uint16_t fill = selected ? accent : panel;
    const uint16_t border = selected ? TFT_WHITE : soft;
    const uint16_t label = selected ? TFT_BLACK : text;
    //Print game name and label
    menuTft.fillRoundRect(20, y, 280, 28, 8, fill);
    menuTft.drawRoundRect(20, y, 280, 28, 8, border);
    menuTft.setTextColor(label, fill);
    menuTft.setTextSize(2);
    menuTft.setCursor(30, y + 7);
    menuTft.println(kGames[i].name);
  }
  //Progress bar
  menuTft.fillRoundRect(20, 225, 280, 12, 6, panel);
  menuTft.fillRoundRect(20, 225, static_cast<int>((selectedIndex + 1) * 280 / kGameCount), 12, 6, accent);
  menuDirty = false;
}

void enterMenu()
{
  activeGame = nullptr;
  menuTft.init();
  menuTft.setRotation(1);
  menuDirty = true;
}

void launchSelectedGame()
{
  activeGame = &kGames[selectedIndex]; //Pointer point to game adress
  activeGame->enter();
}

void returnToMenu()
{
  if (activeGame != nullptr)
  {
    activeGame->exit();
  }
  for (int i = 0; i < kButtonCount; i++) stablePressed[i] = false;
  enterMenu();
}

void handleButtonPress(console::Button button, unsigned long now)
{
  if (button == console::Button::Reset)
  {
    resetPressedAt = now;
    resetLongHandled = false;
  }

  if (activeGame == nullptr) //Chua chon game
  {
    switch (button)
    {
      case console::Button::Up:
        selectedIndex = (selectedIndex + kGameCount - 1) % kGameCount; //Xoay vong
        menuDirty = true;
        break;
      case console::Button::Down:
        selectedIndex = (selectedIndex + 1) % kGameCount; //Xoay vong
   
        menuDirty = true;
        break;
      case console::Button::Right:
        launchSelectedGame();
        break;
      case console::Button::Left: //Fall through default case
      case console::Button::Reset:
        break;
    }
    return;
  }
  //Game playing
  activeGame->onPress(button);
}

void enterDeepSleep();

void handleButtonRelease(console::Button button)
{
  if (button == console::Button::Reset)
  {
    bool handledLongReset = resetLongHandled;
    resetLongHandled = false;
    if (activeGame != nullptr && !handledLongReset)
    {
      activeGame->onPress(button);
    }
    return;
  }

  if (activeGame != nullptr)
  {
    activeGame->onRelease(button);
  }
}

void handleInterrupts()
{
  if (buttonEventQueue == nullptr)
  {
    return;
  }

  ButtonEvent event;
  while (xQueueReceive(buttonEventQueue, &event, 0) == pdPASS)
  {
    if (event.buttonIdx >= kButtonCount)
    {
      continue;
    }

    if (stablePressed[event.buttonIdx] != event.pressed)
    {
      stablePressed[event.buttonIdx] = event.pressed;

      if (stablePressed[event.buttonIdx])
      {
        handleButtonPress(kButtons[event.buttonIdx], event.atMs);
      }
      else
      {
        handleButtonRelease(kButtons[event.buttonIdx]);
      }
    }
  }

  // Prevent stuck
  for (int i = 0; i < kButtonCount; ++i) {
    if (stablePressed[i] && digitalRead(kButtonPins[i]) == HIGH) {
      stablePressed[i] = false;
      handleButtonRelease(kButtons[i]);
    }
  }
}


void enterDeepSleep()
{
  menuTft.fillScreen(TFT_BLACK);
  menuTft.setCursor(90, 110);
  menuTft.setTextColor(TFT_WHITE);
  menuTft.setTextSize(2);
  menuTft.println("Sleeping...");

  delay(200);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0); 
  while(digitalRead(kResetPin)==LOW) delay(10);
  delay(50);
  digitalWrite(kBL, LOW);
  esp_deep_sleep_start();
}

void handleLongReset()
{
  if (!stablePressed[4] || resetLongHandled) //Released or handled long reset
  {
    return;
  }

  if (millis() - resetPressedAt >= kMenuHoldMs)
  {
    resetLongHandled = true;
    if (activeGame == nullptr)
    {
      enterDeepSleep();
    }
    else
    {
      returnToMenu();
    }
  }
}

}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);
  btStop();

  pinMode(kUpPin, INPUT_PULLUP);
  pinMode(kDownPin, INPUT_PULLUP);
  pinMode(kLeftPin, INPUT_PULLUP);
  pinMode(kRightPin, INPUT_PULLUP);
  pinMode(kResetPin, INPUT_PULLUP);
  pinMode(kBL, OUTPUT);

  buttonEventQueue = xQueueCreate(kButtonQueueLength, sizeof(ButtonEvent));
  
  for (int i = 0; i < kButtonCount; ++i)
  {
    void* idxToPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(i));
    attachInterruptArg(digitalPinToInterrupt(kButtonPins[i]), buttonISR, idxToPtr, CHANGE);
  }

  digitalWrite(kBL, HIGH);
  randomSeed(micros()); //Better random numbers
  enterMenu();
}



void loop()
{
  handleInterrupts();
  handleLongReset();
  if (activeGame == nullptr)
  {
    if (menuDirty)
    {
      drawMenu();
    }
    return;
  }


  if (activeGame != nullptr)
  {
    activeGame->tick();
  }

}
