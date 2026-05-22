#include "flappyBird/wifi_time.h"
#include <esp_wifi.h>


namespace flappy_bird{

bool wifiConnected = false;


namespace {

void shutdownWifi()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    esp_wifi_stop();
    esp_wifi_deinit();
}

}

bool getTime()
{
    struct tm timeinfo;
    return syncTime(timeinfo);
}

bool syncTime(tm &timeinfo)
{
    timeinfo = {};
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
    {
        delay(100);
    }

    if (WiFi.status() != WL_CONNECTED)
    {   
        shutdownWifi();
        Serial.println("time wifi failed");
        wifiConnected = false;
        return false;
    }

    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, "pool.ntp.org", "time.google.com", "time.cloudflare.com");
    bool synced = getLocalTime(&timeinfo, 10000);
    if (!synced || timeinfo.tm_year + 1900 < 2024)
    {
        shutdownWifi();
        Serial.println("time sync failed");
        wifiConnected = false;
        return false;
    }

    Serial.printf(
        "time synced: %04d-%02d-%02d %02d:%02d:%02d\n",
        timeinfo.tm_year + 1900,
        timeinfo.tm_mon + 1,
        timeinfo.tm_mday,
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec
    );
    wifiConnected = true;
    shutdownWifi();
    return true;
}

}
