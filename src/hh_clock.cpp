#include "hh_clock.h"
#include "Arduino.h" //debug

const unsigned long secsPerDisplayUpdate = 10;
const unsigned long secsPerNtcUpdate = 10 * 60;

void setup() {
  Motor.start(D8, D7, D6, D5, D4, D3, D0, RX);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  wifi = new HHWifi("The Wifi", "8 characters or longer with CAPITAL LETTERS");
  ntp = new HHNtp("pool.ntp.org");

  Serial.begin(9600); //debug

  data = HHPersistence::get();
  bool restored = (data != NULL);
//  Serial.println("RESTORED: "); //debug
//  Serial.println(restored); //debug

  if (!restored) {
    data = new HHPersistence::HHSchema();

    data->verify = 'H';
    data->version = 'a';

    if (wifi->visualConnect(display) != WL_CONNECTED) {
      while (1) { delay(1); } // not connected; do nothing
    }

    data->epoch = ntp->visualConnect(display);
//    Serial.println("NTP TIME:"); //debug
//    Serial.println(data->epoch); //debug
    if (!ntp->isOnline()) {
      while (1) { delay(1); } // not connected; do nothing
    }

    // set a default alarm and timezone
    data->alarm = 8 * 60; // 8am
    data->timeZone = 1; // UTC+1 British Summer Time

  }

  HHClockFace::Face faces[] = {HHClockFace::Face::FACE_ALARM_INFO, HHClockFace::Face::FACE_IP};
  face = new HHClockFace(faces, sizeof(faces) / sizeof(faces[0]), *data, wifi->localIP());
//  Serial.println("FACES:"); //debug
//  Serial.println(sizeof(faces) / sizeof(faces[0])); //debug

  if (restored) {
    face->show(display, HHClockFace::Face::FACE_WIFICONNECT, data->epoch, data->timeZone);
    
    if (wifi->connect() != WL_CONNECTED) { // try to reconnect to network
      wifi->visualFail(display);
      data->verify = 'G';
      HHPersistence::write(*data);
      while (1) { delay(1); }
    }

    face->ip = wifi->localIP();
    face->show(display, HHClockFace::Face::FACE_NTPCONNECT, data->epoch, data->timeZone);

    unsigned long tempEpoch = ntp->connect();
//    Serial.println("NTP TIME:"); //debug
//    Serial.println(data->epoch); //debug
    if (ntp->isOnline()) {
      data->epoch = tempEpoch;
      if ((data->epoch % 86400) / 60 == data->alarm) {
        alarmRang = true;
      }
    } else {
      face->show(display, HHClockFace::Face::FACE_NTPOFFLINE, data->epoch, data->timeZone);
      delay(secsPerDisplayUpdate * 1000);
    }
  }

  HHServer::beginServer(data);
  
  os_timer_setfn(&clock, onTick, NULL);
  os_timer_arm(&clock, 1000, true);

  // goto loop
}

unsigned int _displayi = 0;
void loop() {
  if (_displayi == 0) {
    _displayi = secsPerDisplayUpdate;
//    Serial.println("\n---\n\nDISPLAY UPDATE"); //debug
//    Serial.println("BETWEEN UPDATES (secs):"); //debug
//    Serial.println(_displayi); //debug

//    Serial.println("TIME:"); //debug
//    Serial.println(data->epoch % 86400); //debug
//    Serial.println("ALARM:"); //debug
//    Serial.println(data->alarm); //debug
//    Serial.println("RANG:"); //debug
//    Serial.println(alarmRang); //debug

    if ((data->epoch % 86400) / 60 == data->alarm && !alarmRang) {
      alarmRang = true;
      
      // dance pony dance
      Motor.right();
      while (1) {
        face->alarmFace(display, data->epoch); // strobe display
        delay(200);
      }
    } else if (ntp->getLastTime() + secsPerNtcUpdate < data->epoch) {
//      Serial.println("NTC UPDATE"); //debug
//      Serial.println("LAST UPDATE:"); //debug
//      Serial.println(ntp->getLastTime() % 86400); //debug
//      Serial.println("BETWEEN UPDATES (secs):"); //debug
//      Serial.println(secsPerNtcUpdate); //debug
      face->show(display, HHClockFace::Face::FACE_NTPCONNECT, data->epoch, data->timeZone);
      unsigned long tempEpoch = ntp->connect();
      if (ntp->isOnline()) {
        data->epoch = tempEpoch;
        HHPersistence::write(*data);
      } else {
        face->show(display, HHClockFace::Face::FACE_NTPOFFLINE, data->epoch, data->timeZone);
        delay(secsPerDisplayUpdate * 1000);
      }
    } else {
//      Serial.println("NEXT FACE"); //debug
      face->next(display, data->epoch, data->timeZone);

      if ((data->epoch % 86400) / 60 > data->alarm) {
        alarmRang = false;
      }
    }
  }

  HHServer::handleClient();

  // goto loop
}

void inline onTick(void *pArg) {
  ++data->epoch;
  --_displayi;
}