#pragma once
#include <WiFi.h>
#include "time.h"

#define WIFI_SSID "Redmi 13"
#define WIFI_PASS "1234567890"
#define GMT_OFFSET_SEC 7*3600
#define DAYLIGHT_OFFSET_SEC 0

bool getTime();
