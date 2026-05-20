#pragma once
#include "flappyBird/config.h"
#include "flappyBird/display.h"

#define MOON_WIDTH 16
#define MOON_HEIGHT 16
#define SUN_HEIGHT 15
#define SUN_WIDTH 15
#define CLOUD_WIDTH 30
#define CLOUD_HEIGHT 30
#define STAR_WIDTH 5
#define STAR_HEIGHT 5

namespace flappy_bird{
    extern const uint16_t moon[] PROGMEM;
    extern const uint16_t sunMorning[] PROGMEM;
    extern const uint16_t sunNoon[] PROGMEM;
    extern const uint16_t cloudMorning1[] PROGMEM;
    extern const uint16_t cloudMorning2[] PROGMEM;
    extern const uint16_t cloudNoon1[] PROGMEM;
    extern const uint16_t cloudNoon2[] PROGMEM;
    extern const uint16_t cloudNight1[] PROGMEM;
    extern const uint16_t cloudNight2[] PROGMEM;
    extern const uint16_t star[] PROGMEM;
}
