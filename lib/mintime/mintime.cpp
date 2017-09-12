#include "mintime.h"

unsigned int Time::hour(unsigned long epoch) {
  return (epoch % 86400L) / 3600;
}

unsigned int Time::hour(unsigned long epoch, int timeZone) {
  return hour(epoch) + timeZone;
}

unsigned int Time::minute(unsigned long epoch) {
  return (epoch % 3600) / 60;
}

unsigned int Time::second(unsigned long epoch) {
  return epoch % 60;
}