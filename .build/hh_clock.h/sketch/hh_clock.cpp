#include "hh_clock.h"

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  wifi = new HHWifi("J-Ph8", "alphabet");
  ntp = new HHNtp("pool.ntp.org");

  if (wifi.visualConnect() != WL_CONNECTED)
    while (1) { } // not connected; do nothing
  }

  epoch = ntp.visualConnect();
  if (!ntp.isOnline()) {
    while (1) { } // not connected; do nothing
  }

  os_timer_setfn(&clock, onTick, NULL);
  os_timer_arm(&clock, 1000, true);

  secsPerDisplayUpdate = 10;
  secsPerNtcUpdate = 10 * 60;

  HHClockFaces faces[] = {HH_CLOCKFACE_ALARM, HH_CLOCKFACE_IP};
  face = new HHClockFace(faces, sizeof(faces) / sizeof(faces[0]));
  
  // goto loop
}

void loop() {
  if (ntp.getLastTime() + secsPerNtcUpdate > epoch) {
    clockFace(HH_CLOCKFACE_NTPCONNECT);
    ntp.connect();
  } else {
    clockFace(face);
    face ^= 1;  // there are two faces so we can just flip the bit.
  }

  delay(secsPerDisplayUpdate * 1000);

  // goto loop
}

void inline onTick(void *pArg) {
  ++epoch;
}