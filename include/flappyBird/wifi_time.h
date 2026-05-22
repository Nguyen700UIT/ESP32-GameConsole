#pragma once
#include <WiFi.h>
#include "time.h"

#define WIFI_SSID "Nguyen"
#define WIFI_PASS "1234567890"
#define GMT_OFFSET_SEC 7*3600
#define DAYLIGHT_OFFSET_SEC 0

namespace flappy_bird
{
    extern bool wifiConnected;


    bool getTime();
    bool syncTime(tm &timeinfo);
}

