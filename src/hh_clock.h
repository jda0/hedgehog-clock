#ifndef _HEDGEHOG_CLOCK_H
#define _HEDGEHOG_CLOCK_H

extern "C" {
  #include "user_interface.h"
}

#include "espDualStepper.h"
#include "hh_wifi.h"
#include "hh_ntp.h"
#include "hh_clockface.h"
#include "hh_rom.h"
#include "hh_server.h"

ESP_SSD1306 display(16);
HHWifi *wifi;
HHNtp *ntp;
HHClockFace *face;

os_timer_t clock;

// int timeZone = 1;
// unsigned long epoch = 0;
// unsigned long alarm = 0;

HHROM::HHSchema *data;

bool alarmRang = false;

void setupDisplay();
void setupWifi();
void setupNtp();
void inline onTick(void *pArg);

#endif // _HEDGEHOG_CLOCK_H
