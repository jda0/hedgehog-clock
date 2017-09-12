#include "hh_ntp.h"

HHNtp::HHNtp(const char *serverName) {
  ntpServerName = serverName;

  ntpPacketSize = 48;
  localPort = 2390;
  online = false;
}

void HHNtp::overrideOnlineFlag(bool flag) {
  online = flag;
}

bool HHNtp::isOnline() {
  return online;
}

unsigned long HHNtp::getLastTime() {
  return lastTime;
}

unsigned long HHNtp::sendNTPpacket(IPAddress& address) {
  // set all bytes in the buffer to 0
  memset(ntpPktBuf, 0, NTP_PKT_SZ);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  ntpPktBuf[0] = 0b11100011;   // LI, Version, Mode
  ntpPktBuf[1] = 0;     // Stratum, or type of clock
  ntpPktBuf[2] = 6;     // Polling Interval
  ntpPktBuf[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  ntpPktBuf[12]  = 49;
  ntpPktBuf[13]  = 0x4E;
  ntpPktBuf[14]  = 49;
  ntpPktBuf[15]  = 52;

  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(ntpPktBuf, NTP_PKT_SZ);
  Udp.endPacket();
}

// TODO: extract duplicate code in connect() & visualConnect()

bool HHNtp::connect() {
  int ok;

  if (WiFi.hostByName(ntpServerName, ntpServerIp)) {
    for (int i = 0; i < 30 && !(ok = Udp.parsePacket()); i++) {
      if (i % 10 == 0) {
        sendNTPpacket(ntpServerIp);
      }

      delay(200);
    }
  }

  if (ok) {
    Udp.read(ntpPktBuf, NTP_PKT_SZ); // read the packet into the buffer
    unsigned long highWord = word(ntpPktBuf[40], ntpPktBuf[41]);
    unsigned long lowWord = word(ntpPktBuf[42], ntpPktBuf[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    
    online = true;
    lastTime = secsSince1900 - seventyYears
    return lastTime;
    // Serial.print("UNX");
    // Serial.println(epoch);
  } else {
    online = false;
    return 0;
  }
}

bool HHNtp::visualConnect(ESP_SSD1306& display) {
  int ok = WiFi.hostByName(ntpServerName, ntpServerIp);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 8);
  display.print("Getting time:");
  display.setTextSize(2);
  display.setCursor(30, 16);

  if (ok) {
    display.print("Try  /3");

    for (int i = 0; i < 30 && !ok; i++) {
      if (i % 10 == 0) {
        sendNTPpacket(ntpServerIp);
        
        display.fillRect(78, 16, 10, 16, BLACK);
        display.drawChar(78, 16, (i / 10) + 0x31, WHITE, WHITE, 2);
      }

      display.fillRect(16, 16, 11, 11, BLACK);
      
      if (i % 4 == 0)
        display.drawXBitmap(16, 16, HHSX_Clock0.bitmap,
            HHSX_Clock0.width, HHSX_Clock0.height, WHITE);
      else if (i % 4 == 1)
        display.drawXBitmap(16, 16, HHSX_Clock1.bitmap,
            HHSX_Clock1.width, HHSX_Clock1.height, WHITE);
      else if (i % 4 == 2)
        display.drawXBitmap(16, 16, HHSX_Clock2.bitmap,
            HHSX_Clock2.width, HHSX_Clock2.height, WHITE);
      else
        display.drawXBitmap(16, 16, HHSX_Clock3.bitmap,
            HHSX_Clock3.width, HHSX_Clock3.height, WHITE);

      display.display();
      delay(200);
      
      ok = Udp.parsePacket();
    }
  }

  if (ok) {
    Udp.read(ntpPktBuf, NTP_PKT_SZ); // read the packet into the buffer
    unsigned long highWord = word(ntpPktBuf[40], ntpPktBuf[41]);
    unsigned long lowWord = word(ntpPktBuf[42], ntpPktBuf[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    
    online = true;
    lastTime = secsSince1900 - seventyYears;
    return lastTime;
    // Serial.print("UNX");
    // Serial.println(epoch);
  } else {
    display.fillRect(16, 16, 96, 16, BLACK);
    display.drawXBitmap(16, 16, HHSX_ClockFail.bitmap,  
        HHSX_ClockFail.width, HHSX_ClockFail.height, WHITE);
    display.setCursor(30, 16);
    display.print("FAILED");
    display.display();

    online = false;
    return 0;
  }
}