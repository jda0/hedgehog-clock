#include <Arduino.h>
#line 1 "c:\\Users\\J\\Documents\\Arduino\\hedgehog_alarm_2\\src\\hh_clock.h"
#line 1 "c:\\Users\\J\\Documents\\Arduino\\hedgehog_alarm_2\\src\\hh_clock.h"
#ifndef _HEDGEHOG_CLOCK_H
#define _HEDGEHOG_CLOCK_H

#include <hh_wifi.h>
#include <hh_ntp.h>
#include <hh_clockface.h>
#include <mintime.h>

ESP_SSD1306 display(16);
HHWifi wifi;
HHNtp ntp;

os_timer_t clock;

byte timeZone;
unsigned long epoch;
unsigned long alarm;
HHClockFace face;

const unsigned long secsPerDisplayUpdate;
const unsigned long secsPerNtcUpdate;

void setupDisplay();
void setupWifi();
void setupNtp();
void clockFace(ESP_SSD1306& display, HHClockFace face);
void inline onTick(void *pArg);

#endif // _HEDGEHOG_CLOCK_H

