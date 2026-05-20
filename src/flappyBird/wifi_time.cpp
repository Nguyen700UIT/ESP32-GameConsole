#include "flappyBird/wifi_time.h"

bool getTime()
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
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        Serial.println("failed");
        return false;
    }
    Serial.println("connected");

    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, "pool.ntp.org");
    tm timeinfo;
    bool synced = getLocalTime(&timeinfo, 3000);

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    return synced;
}



