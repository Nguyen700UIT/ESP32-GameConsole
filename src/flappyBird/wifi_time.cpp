#include "flappyBird/wifi_time.h"
#include <esp_wifi.h>


namespace flappy_bird{



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
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000)
    {
        delay(100);
    }

    if (WiFi.status() != WL_CONNECTED)
    {   
        shutdownWifi();
        Serial.println("failed");
        return false;
    }
    Serial.println("connected");

    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, "pool.ntp.org");
    bool synced = getLocalTime(&timeinfo, 3000);

    shutdownWifi();
    return synced;
}

}
