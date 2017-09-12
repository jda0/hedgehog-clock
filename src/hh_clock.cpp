#include "hh_clock.h"

const unsigned long secsPerDisplayUpdate = 10;
const unsigned long secsPerNtcUpdate = 10 * 60;

void setup() {
  Motor.start(D8, D7, D6, D5, D4, D3, D0, RX);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  wifi = new HHWifi("J-Ph8", "alphabet");
  ntp = new HHNtp("pool.ntp->org");

  data = HHPersistence::get();
  bool restored = (data != NULL);

  if (!restored) {
    data = new HHPersistence::HHSchema();

    if (wifi->visualConnect(display) != WL_CONNECTED) {
      while (1) { delay(1); } // not connected; do nothing
    }

    data->epoch = ntp->visualConnect(display);
    if (!ntp->isOnline()) {
      while (1) { delay(1); } // not connected; do nothing
    }

    // set a default alarm and timezone
    data->alarm = 8 * 3600; // 8am
    data->timeZone = 1; // UTC+1 British Summer Time
  } else {
    
  }

  os_timer_setfn(&clock, onTick, NULL);
  os_timer_arm(&clock, 1000, true);

  HHClockFace::Face faces[] = {HHClockFace::Face::FACE_ALARM_INFO, HHClockFace::Face::FACE_IP};
  face = new HHClockFace(faces, sizeof(faces) / sizeof(faces[0]), data->alarm, data->timeZone, wifi->localIP());

  if (restored) {
    face->show(display, HHClockFace::Face::FACE_WIFICONNECT, data->epoch);
    
    if (wifi->connect() != WL_CONNECTED) { // try to reconnect to network
      wifi->visualFail(display);
      while (1) { delay(1); }
    }

    face->show(display, HHClockFace::Face::FACE_NTPCONNECT, data->epoch);

    unsigned long tempEpoch = ntp->connect();
    if (ntp->isOnline()) {
      data->epoch = tempEpoch;
    } else {
      face->show(display, HHClockFace::Face::FACE_NTPOFFLINE, data->epoch);
      delay(secsPerDisplayUpdate * 1000);
    }
  }

  // goto loop
}

void loop() {
  if (data->epoch % 86400 > data->alarm && !alarmRang) {
    alarmRang = true;

    // dance pony dance
    Motor.right();
    while (1) {
      face->alarmFace(display, data->epoch); // strobe display
      delay(200);
    }
  } else if (ntp->getLastTime() + secsPerNtcUpdate > data->epoch) {
    face->show(display, HHClockFace::Face::FACE_NTPCONNECT, data->epoch);
    unsigned long tempEpoch = ntp->connect();
    if (ntp->isOnline()) {
      data->epoch = tempEpoch;
    }
  } else {
    face->next(display, data->epoch);
  }

  delay(secsPerDisplayUpdate * 1000);

  // goto loop
}

void inline onTick(void *pArg) {
  ++data->epoch;
}