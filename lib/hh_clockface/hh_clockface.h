#ifndef _HEDGEHOG_CLOCKFACE_H
#define _HEDGEHOG_CLOCKFACE_H

#include "string.h"
#include "ESP_SSD1306.h"
#include "hh_persistence.h"
#include "hh_symbols.h"
#include "mintime.h"

class HHClockFace {
public:
  enum Face {
    FACE_IP,
    FACE_ALARM_INFO,
    FACE_NTPCONNECT,
    FACE_NTPOFFLINE,
    FACE_WIFICONNECT,
    FACE_ALARM_RINGING,
    FACE_ALARM_RINGING_INVERSE
  };
  
  HHClockFace(Face *faces, int n, HHPersistence::HHSchema &data, const char *ip);
  void show(ESP_SSD1306& display, Face face, unsigned long epoch);
  void next(ESP_SSD1306& display, unsigned long epoch);
  void alarmFace(ESP_SSD1306& display, unsigned long epoch);

  HHPersistence::HHSchema * data;
  const char *ip;
protected:
  Face *_faces;
  int _n;
  int _face;
};

#endif // _HEDGEHOG_CLOCKFACE_H
