#ifndef _HEDGEHOG_WIFI_H
#define _HEDGEHOG_WIFI_H

#include "ESP8266WiFi.h"
#include "ESP_SSD1306.h"
#include "hh_symbols.h"

class HHWifi {
public:
  HHWifi(const char *ssid, const char *pass);
  wl_status_t connect();
  wl_status_t visualConnect(ESP_SSD1306& display);
  void visualFail(ESP_SSD1306& display);
  const char * localIP();
protected:
  const char *_ssid;
  const char *_pass;
  char _ip[16];
};

#endif // _HEDGEHOG_WIFI_H
