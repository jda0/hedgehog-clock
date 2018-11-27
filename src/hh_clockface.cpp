#include "hh_clockface.h"

HHClockFace::HHClockFace(Face *faces, int n, HHROM::HHSchema &data, const char *ip) {
  if (n > 0) {
    _faces = new Face[n];
    memcpy(_faces, faces, n * sizeof(faces[0]));
  }
  _n = n;
  _face = 0;
  this->data = &data;
  this->ip = ip;
}

void HHClockFace::next(ESP_SSD1306& display, unsigned long epoch, int tz) {
  ++_face %= _n;
  show(display, _faces[_face], epoch, tz);
}

void HHClockFace::next(ESP_SSD1306& display, unsigned long epoch) {
  next(display, epoch, 0);
}

void HHClockFace::show(ESP_SSD1306& display, Face face, unsigned long epoch, int tz) {
  unsigned int temp;

  display.clearDisplay();
  display.drawRect(16, 11, 96, 32, WHITE);
  display.drawRect(17, 12, 94, 30, WHITE);
  display.fillRect(61, 22, 6, 6, WHITE);  // separator
  display.fillRect(61, 31, 6, 6, WHITE);  // separator

  display.setTextSize(3);

  temp = Time::hour(epoch, tz);
  display.setCursor(22, 16);
  display.write(temp / 10 + '0');
  display.write(temp % 10 + '0');

  temp = Time::minute(epoch);
  display.setCursor(73, 16);
  display.write(temp / 10 + '0');
  display.write(temp % 10 + '0');
  display.setTextSize(1);

  switch (face) {
    case FACE_IP:
      display.drawXBitmap(55 - strlen(ip) * 3, 47, HHSX_Wifi2.bitmap, 
          HHSX_Wifi2.w, HHSX_Wifi2.h, WHITE);
      display.setCursor(73 - strlen(ip) * 3, 48);
      display.print(ip);
      break;

    case FACE_ALARM_INFO:
      display.drawXBitmap(42, 47, HHSX_Alarm.bitmap, 
          HHSX_Alarm.w, HHSX_Alarm.h, WHITE);

      temp = data->alarm / 60;
      display.setCursor(56, 48);
      display.write(temp / 10 + '0');
      display.write(temp % 10 + '0');
      
      display.write(':');

      temp = data->alarm % 60;
      display.write(temp / 10 + '0');
      display.write(temp % 10 + '0');
      break;

    case FACE_NTPCONNECT:
      display.drawXBitmap(31, 47, HHSX_Clock1.bitmap, 
          HHSX_Clock1.w, HHSX_Clock1.h, WHITE);

      display.setCursor(49, 48);
      display.print("UPDATING");
      break;

    case FACE_NTPOFFLINE:
      display.drawXBitmap(34, 47, HHSX_ClockFail.bitmap, 
          HHSX_ClockFail.w, HHSX_ClockFail.h, WHITE);

      display.setCursor(52, 48);
      display.print("OFFLINE");
      break;

    case FACE_WIFICONNECT:
      display.drawXBitmap(22, 50, HHSX_Wifi1.bitmap, 
          HHSX_Wifi1.w, HHSX_Wifi1.h, WHITE);

      display.setCursor(37, 48);
      display.print("RECONNECTING");
      break;

    case FACE_ALARM_RINGING:
      display.drawXBitmap(31, 47, HHSX_Alarm.bitmap, 
          HHSX_Alarm.w, HHSX_Alarm.h, WHITE);
      
      display.setCursor(49, 48);
      display.print("RINGING!");
      break;

    case FACE_ALARM_RINGING_INVERSE:
      display.fillRect(0, 46, 128, 18, WHITE);
      display.drawXBitmap(31, 47, HHSX_Alarm.bitmap, 
          HHSX_Alarm.w, HHSX_Alarm.h, BLACK);
      
      display.setTextColor(BLACK);
      display.setCursor(49, 48);
      display.print("RINGING!");
      display.setTextColor(WHITE);
      break;
  }

  display.display();
}

void HHClockFace::show(ESP_SSD1306& display, Face face, unsigned long epoch) {
  show(display, face, epoch, 0);
}

void HHClockFace::alarmFace(ESP_SSD1306& display, unsigned long epoch, int tz) {
  ++_face %= 2;
  show(display, _face ? FACE_ALARM_RINGING : FACE_ALARM_RINGING_INVERSE, epoch, tz);
}

void HHClockFace::alarmFace(ESP_SSD1306& display, unsigned long epoch) {
  alarmFace(display, epoch, 0);
}
