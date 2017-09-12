#include <Arduino.h>
#line 1 "c:\\Users\\J\\Documents\\Arduino\\hedgehog_alarm_2\\lib\\hh_ntp\\hh_ntp.h"
#line 1 "c:\\Users\\J\\Documents\\Arduino\\hedgehog_alarm_2\\lib\\hh_ntp\\hh_ntp.h"
#ifndef _HEDGEHOG_NTP_H
#define _HEDGEHOG_NTP_H

#include "string.h"
// #include "ESP8266WiFi.h"
// #include "WifiUdp.h"
#include "ESP_SSD1306.h"
#include "hh_symbols.h"

class HHNtp {
public:
  HHNtp(const char *serverName);
  void overrideOnlineFlag(bool flag);
  bool isOnline();
  unsigned long getLastTime();
  unsigned long connect();
  unsigned long visualConnect(ESP_SSD1306& display);
protected:
  const int ntpPacketSize;
  const unsigned int localPort;
  const char *ntpServerName;
  IPAddress ntpServerIp;
  bool online;
  unsigned long lastTime;

  static unsigned long sendNTPpacket(IPAddress& address);
}

#endif // _HEDGEHOG_NTP_H

