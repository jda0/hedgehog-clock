#ifndef _MINTIME_H
#define _MINTIME_H

namespace Time {
  unsigned int hour(unsigned long epoch);
  unsigned int hour(unsigned long epoch, int timeZone);
  unsigned int minute(unsigned long epoch);
  unsigned int second(unsigned long epoch);
};

#endif // _MINTIME_H