#ifndef _HEDGEHOG_NTP_H
#define _HEDGEHOG_NTP_H

#include "string.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
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
  int ntpPacketSize;
  unsigned char *buffer;
  unsigned int localPort;
  const char *ntpServerName;
  IPAddress ntpServerIp;
  WiFiUDP Udp;
  bool online;
  unsigned long lastTime;

  void sendNTPPacket(IPAddress& address);
};

#endif // _HEDGEHOG_NTP_H
