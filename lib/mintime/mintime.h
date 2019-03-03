#ifndef _MINTIME_H
#define _MINTIME_H

namespace Time {
  unsigned int hour(unsigned long epoch);
  unsigned int hour(unsigned long epoch, int timeZone);
  unsigned int minute(unsigned long epoch);
  unsigned int minuteOfDay(unsigned long epoch);
  unsigned int minuteOfDay(unsigned long epoch, int timeZone);
  unsigned int second(unsigned long epoch);
  unsigned long secondOfDay(unsigned long epoch);
  unsigned long secondOfDay(unsigned long epoch, int timeZone);
};

#endif // _MINTIME_H