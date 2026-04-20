#include <Arduino.h>
#include <WiFi.h>
#include <esp_bt.h>
#include <TFT_eSPI.h>

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
constexpr unsigned long kDebounceMs = 35;
constexpr unsigned long kMenuHoldMs = 1200;

struct GameEntry {
  const char* name;
  //Function pointers
  void (*enter)(); 
  void (*tick)();
  void (*exit)();
  void (*onPress)(console::Button button);
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
  {"Flappy Bird", flappy_bird::enter, flappy_bird::tick, flappy_bird::exit, flappy_bird::onButtonPressed},
  {"Snake", snake::enter, snake::tick, snake::exit, snake::onButtonPressed},
  {"Pong", pong::enter, pong::tick, pong::exit, pong::onButtonPressed},
  {"Tetris", tetris::enter, tetris::tick, tetris::exit, tetris::onButtonPressed},
};

constexpr size_t kGameCount = sizeof(kGames) / sizeof(kGames[0]);

TFT_eSPI menuTft;
const GameEntry* activeGame = nullptr;
size_t selectedIndex = 0;
bool menuDirty = true;
bool stablePressed[kButtonCount] = {false};
bool rawPressed[kButtonCount] = {false};
unsigned long lastChangeAt[kButtonCount] = {0};
unsigned long resetPressedAt = 0;
bool resetLongHandled = false;

uint16_t rgb(uint8_t r, uint8_t g, uint8_t b)
{
  return menuTft.color565(r, g, b);
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
  enterMenu();
}

void handleButtonPress(console::Button button, unsigned long now)
{
  if (button == console::Button::Reset)
  {
    resetPressedAt = now;
    resetLongHandled = false;
    if (activeGame != nullptr)
    {
      return;
    }
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

void handleButtonRelease(console::Button button)
{
  if (button == console::Button::Reset)
  {
    if (activeGame != nullptr && !resetLongHandled) //Not returning menu if not hold reset for long
    {
      activeGame->onPress(button); 
    }
    resetLongHandled = false;
  }
}

void pollButtons()
{
  const unsigned long now = millis();

  for (uint8_t i = 0; i < kButtonCount; ++i)
  {
    const bool currentRaw = (digitalRead(kButtonPins[i]) == LOW);
    if (currentRaw != rawPressed[i])
    {
      rawPressed[i] = currentRaw; //Read changed button state before debouncing
      lastChangeAt[i] = now; //Save time button state changed
    }

    if ((now - lastChangeAt[i] >= kDebounceMs) && (stablePressed[i] != currentRaw)) //After debounce time for that button check if button still at that state
    {
      stablePressed[i] = currentRaw;
      if (stablePressed[i])
      {
        handleButtonPress(kButtons[i], now);
      }
      else
      {
        handleButtonRelease(kButtons[i]);
      }
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
  if (!stablePressed[4] || resetLongHandled) //No press reset or handled long reset
  {
    return;
  }

  if (millis() - resetPressedAt >= kMenuHoldMs)
  {
    if (activeGame == nullptr)
    {
      enterDeepSleep();
    }
    else
    {
      returnToMenu();
    }
    resetLongHandled = true;
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

  digitalWrite(kBL, HIGH);
  randomSeed(micros()); //Better random numbers
  enterMenu();
}



void loop()
{
  pollButtons();
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

