#include <Arduino.h>
#line 1 "c:\\Users\\J\\Documents\\Arduino\\hedgehog_alarm_2\\lib\\hh_wifi\\hh_wifi.cpp"
#line 1 "c:\\Users\\J\\Documents\\Arduino\\hedgehog_alarm_2\\lib\\hh_wifi\\hh_wifi.cpp"
#include "hh_wifi.h"

HH_Wifi::HH_Wifi(const char *ssid, const char *pass) {
  _ssid = ssid;
  _pass = pass;
}

wl_status_t connect() {
  WiFi.begin(ssid, pass);

  while (WiFi.status() == WL_DISCONNECTED) {
    delay(1);
  }
  
  return WiFi.status();
}

wl_status_t visualConnect() {
  WiFi.begin(ssid, pass);
  
  display1.clearDisplay();
  display1.setTextSize(1);
  display1.setCursor(30, 8);
  display1.print("Connecting to");
  display1.setTextSize(2);
  String _disp = String(ssid) + "  " + ssid;

  int j = 0;
  for (int i = 0;
        WiFi.status() == WL_DISCONNECTED || i < 3;
        ++i %= 4, ++j %= strlen(ssid) + 2) {
    
    display1.fillRect(16, 16, 96, 16, BLACK);

    if (i == 0)
      display1.drawXBitmap(26, 26, HHSX_Wifi0.bitmap, 2, 2, WHITE);
    else if (i == 1)
      display1.drawXBitmap(22, 22, HHSX_Wifi1.bitmap, 6, 6, WHITE);
    else if (i == 2)
      display1.drawXBitmap(19, 19, HHSX_Wifi2.bitmap, 9, 9, WHITE);
    else
      display1.drawXBitmap(16, 16, HHSX_Wifi3.bitmap, 12, 12, WHITE);
    
    display1.setCursor(30, 16);
    if (strlen(ssid) > 7) {
      display1.print(_disp.substring(j, j + 7));
    } else {
      display1.print(ssid);
    }

    display1.display();
    delay(200);
  }

  if (WiFi.status() != WL_CONNECTED) {
    _disp = _disp.substring(j, j + 6) + "FAILED ";
    
    for (j = 0; j < 7; j++) {
      display1.fillRect(16, 16, 96, 16, BLACK);

      if (j == 0)
        display1.drawXBitmap(16, 16, HHSX_WifiFail0.bitmap, 12, 12, WHITE);
      else if (j == 1)
        display1.drawXBitmap(16, 16, HHSX_WifiFail1.bitmap, 12, 12, WHITE);
      else if (j == 2)
        display1.drawXBitmap(16, 16, HHSX_WifiFail2.bitmap, 12, 12, WHITE);
      else if (j == 3)
        display1.drawXBitmap(16, 16, HHSX_WifiFail3.bitmap, 12, 12, WHITE);
      else if (j == 4)
        display1.drawXBitmap(16, 16, HHSX_WifiFail4.bitmap, 12, 12, WHITE);
      else
        display1.drawXBitmap(16, 16, HHSX_WifiFail5.bitmap, 12, 12, WHITE);

      if (j < 5 && j != 2)
        display1.fillRect(102 - j * 6, 8, 5, 8, BLACK);
      else if (j == 5)
        display1.drawChar(78, 8, 'o', WHITE, WHITE, 1);
      else if (j == 6)
        display1.drawChar(84, 8, 'n', WHITE, WHITE, 1);

      display1.setCursor(30, 16);
      display1.print(_disp.substring(j, j + 7));
      
      display1.display();
      delay(200);
    }
  }

  return WiFi.status();
}
