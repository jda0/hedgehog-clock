#ifndef _HEDGEHOG_ROM_H
#define _HEDGEHOG_ROM_H

#include "EEPROM.h"

namespace HHROM {
  struct HHSchema {
    char verify;
    char version;
    char timeZone;
    unsigned long epoch;
    unsigned long alarm;
  };

  HHSchema * get();
  bool write(HHSchema &data);
}

#endif // _HEDGEHOG_ROM_H
