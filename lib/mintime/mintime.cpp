#include "mintime.h"

unsigned int Time::hour(unsigned long epoch) {
  return secondOfDay(epoch) / 3600;
}

unsigned int Time::hour(unsigned long epoch, int timeZone) {
  return secondOfDay(epoch, timeZone) / 3600;
}

unsigned int Time::minute(unsigned long epoch) {
  return (epoch % 3600) / 60;
}

unsigned int Time::minuteOfDay(unsigned long epoch) {
  return secondOfDay(epoch) / 60;
}

unsigned int Time::minuteOfDay(unsigned long epoch, int timeZone) {
  return secondOfDay(epoch, timeZone) / 60;
}

unsigned int Time::second(unsigned long epoch) {
  return epoch % 60;
}

unsigned long Time::secondOfDay(unsigned long epoch) {
  return epoch % 86400L;
}

unsigned long Time::secondOfDay(unsigned long epoch, int timeZone) {
  return (epoch + timeZone * 3600) % 86400L;
}