#ifndef _HEDGEHOG_WIFI_H
#define _HEDGEHOG_WIFI_H

#include "ESP8266WiFi.h"
#include "ESP_SSD1306.h"

class HH_Wifi {
public:
  HH_Wifi(const char *ssid, const char *pass);
  wl_status_t connect();
  wl_status_t visualConnect(ESP_SSD1306 display);
protected:
  const char *_ssid;
  const char *_pass;
}

#endif // _HEDGEHOG_WIFI_H
