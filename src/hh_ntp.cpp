#include "hh_ntp.h"
//#include "Arduino.h" //debug

HHNtp::HHNtp(const char *serverName) {
  ntpServerName = serverName;

  ntpPacketSize = 48;
  localPort = 2390;
  online = false;

  buffer = new unsigned char[ntpPacketSize];
}

void HHNtp::overrideOnlineFlag(bool flag) {
  online = flag;
}

bool HHNtp::isOnline() {
//  Serial.println("NTP ONLINE:"); //debug
//  Serial.println(online); //debug
  return online;
}

unsigned long HHNtp::getLastTime() {
  return lastTime;
}

unsigned long HHNtp::sendNTPpacket(IPAddress& address) {
  // set all bytes in the buffer to 0
  memset(buffer, 0, ntpPacketSize);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  buffer[0] = 0b11100011;   // LI, Version, Mode
  buffer[1] = 0;     // Stratum, or type of clock
  buffer[2] = 6;     // Polling Interval
  buffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  buffer[12]  = 49;
  buffer[13]  = 0x4E;
  buffer[14]  = 49;
  buffer[15]  = 52;

  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(buffer, ntpPacketSize);
  Udp.endPacket();
}

// TODO: extract duplicate code in connect() & visualConnect()

unsigned long HHNtp::connect() {
  int ok;
  Udp.begin(localPort);

  if (WiFi.hostByName(ntpServerName, ntpServerIp)) {
    ok = false;
    for (int i = 0; i < 30 && !ok; i++) {
      if (i % 10 == 0) {
        sendNTPpacket(ntpServerIp);
      }

      ok = Udp.parsePacket();
      delay(200);
    }
  }

  if (ok) {
    Udp.read(buffer, ntpPacketSize); // read the packet into the buffer
    unsigned long highWord = word(buffer[40], buffer[41]);
    unsigned long lowWord = word(buffer[42], buffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    
    online = true;
    lastTime = secsSince1900 - seventyYears;
    return lastTime;
    // Serial.print("UNX");
    // Serial.println(epoch);
  } else {
    online = false;
    return 0;
  }
}

unsigned long HHNtp::visualConnect(ESP_SSD1306& display) {
  Udp.begin(localPort);
  int ok = WiFi.hostByName(ntpServerName, ntpServerIp);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 8);
  display.print("Getting time:");
  display.setTextSize(2);
  display.setCursor(30, 16);

  if (ok) {
    display.print("Try  /3");

    ok = false;
    for (int i = 0; i < 30 && !ok; i++) {
      if (i % 10 == 0) {
        sendNTPpacket(ntpServerIp);
        
        display.fillRect(78, 16, 10, 16, BLACK);
        display.drawChar(78, 16, (i / 10) + 0x31, WHITE, WHITE, 2);
      }

      display.fillRect(16, 16, 11, 11, BLACK);
      
      if (i % 4 == 0)
        display.drawXBitmap(16, 16, HHSX_Clock0.bitmap,
            HHSX_Clock0.w, HHSX_Clock0.h, WHITE);
      else if (i % 4 == 1)
        display.drawXBitmap(16, 16, HHSX_Clock1.bitmap,
            HHSX_Clock1.w, HHSX_Clock1.h, WHITE);
      else if (i % 4 == 2)
        display.drawXBitmap(16, 16, HHSX_Clock2.bitmap,
            HHSX_Clock2.w, HHSX_Clock2.h, WHITE);
      else
        display.drawXBitmap(16, 16, HHSX_Clock3.bitmap,
            HHSX_Clock3.w, HHSX_Clock3.h, WHITE);

      display.display();
      ok = Udp.parsePacket();
      delay(200);
    }
  }

  if (ok) {
    Udp.read(buffer, ntpPacketSize); // read the packet into the buffer
    unsigned long highWord = word(buffer[40], buffer[41]);
    unsigned long lowWord = word(buffer[42], buffer[43]);
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
        HHSX_ClockFail.w, HHSX_ClockFail.h, WHITE);
    display.setCursor(30, 16);
    display.print("FAILED");
    display.display();

    online = false;
    return 0;
  }
}