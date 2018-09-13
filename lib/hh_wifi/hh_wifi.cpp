#include "hh_wifi.h"

HHWifi::HHWifi(const char *ssid, const char *pass) {
  _ssid = ssid;
  _pass = pass;
  strcpy(_ip, "");
}

// TODO: extract duplicate code in connect() & visualConnect()

wl_status_t HHWifi::connect() {
  WiFi.begin(_ssid, _pass);

  while (WiFi.status() == WL_DISCONNECTED) {
    delay(1);
  }
  
  IPAddress ip = WiFi.localIP();
  sprintf(_ip, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

  return WiFi.status();
}

wl_status_t HHWifi::visualConnect(ESP_SSD1306& display) {
  WiFi.begin(_ssid, _pass);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 8);
  display.print("Connecting to");
  display.setTextSize(2);
  String _disp = String(_ssid) + "  " + _ssid;

  int j = 0;
  for (int i = 0;
        WiFi.status() == WL_DISCONNECTED || i < 3;
        ++i %= 4, ++j %= strlen(_ssid) + 2) {
    
    display.fillRect(16, 16, 96, 16, BLACK);

    if (i == 0)
      display.drawXBitmap(26, 26, HHSX_Wifi0.bitmap, 
          HHSX_Wifi0.w, HHSX_Wifi0.h, WHITE);
    else if (i == 1)
      display.drawXBitmap(22, 22, HHSX_Wifi1.bitmap, 
          HHSX_Wifi1.w, HHSX_Wifi1.h, WHITE);
    else if (i == 2)
      display.drawXBitmap(19, 19, HHSX_Wifi2.bitmap, 
          HHSX_Wifi2.w, HHSX_Wifi2.h, WHITE);
    else
      display.drawXBitmap(16, 16, HHSX_Wifi3.bitmap, 
          HHSX_Wifi3.w, HHSX_Wifi3.h, WHITE);
    
    display.setCursor(30, 16);
    if (strlen(_ssid) > 7) {
      display.print(_disp.substring(j, j + 7));
    } else {
      display.print(_ssid);
    }

    IPAddress ip = WiFi.localIP();
    sprintf(_ip, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    display.display();
    delay(200);
  }

  if (WiFi.status() != WL_CONNECTED) {
    _disp = _disp.substring(j, j + 6) + "FAILED ";
    
    for (j = 0; j < 7; j++) {
      display.fillRect(16, 16, 96, 16, BLACK);
  
      if (j == 0)
        display.drawXBitmap(16, 16, HHSX_WifiFail0.bitmap, 
            HHSX_WifiFail0.w, HHSX_WifiFail0.h, WHITE);
      else if (j == 1)
        display.drawXBitmap(16, 16, HHSX_WifiFail1.bitmap, 
            HHSX_WifiFail1.w, HHSX_WifiFail1.h, WHITE);
      else if (j == 2)
        display.drawXBitmap(16, 16, HHSX_WifiFail2.bitmap, 
            HHSX_WifiFail2.w, HHSX_WifiFail2.h, WHITE);
      else if (j == 3)
        display.drawXBitmap(16, 16, HHSX_WifiFail3.bitmap, 
            HHSX_WifiFail3.w, HHSX_WifiFail3.h, WHITE);
      else if (j == 4)
        display.drawXBitmap(16, 16, HHSX_WifiFail4.bitmap, 
            HHSX_WifiFail4.w, HHSX_WifiFail4.h, WHITE);
      else
        display.drawXBitmap(16, 16, HHSX_WifiFail5.bitmap, 
            HHSX_WifiFail5.w, HHSX_WifiFail5.h, WHITE);
  
      if (j < 5 && j != 2)
        display.fillRect(102 - j * 6, 8, 5, 8, BLACK);
      else if (j == 5)
        display.drawChar(78, 8, 'o', WHITE, WHITE, 1);
      else if (j == 6)
        display.drawChar(84, 8, 'n', WHITE, WHITE, 1);
  
      display.setCursor(30, 16);
      display.print(_disp.substring(j, j + 7));
      
      display.display();
      delay(200);
    }
  }

  return WiFi.status();
}

void HHWifi::visualFail(ESP_SSD1306& display) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(30, 8);
  display.print("Connection");
  display.setTextSize(2);
  
  display.drawXBitmap(16, 16, HHSX_WifiFail5.bitmap, 
      HHSX_WifiFail5.w, HHSX_WifiFail5.h, WHITE);
  
  display.setCursor(30, 16);
  display.print("FAILED");
  
  display.display();
  delay(200);
}

const char * HHWifi::localIP() {
  return _ip;
}