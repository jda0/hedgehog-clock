#ifndef _HEDGEHOG_PERSISTENCE_H
#define _HEDGEHOG_PERSISTENCE_H

#include "EEPROM.h"

namespace HHPersistence {
  struct HHSchema {
    char verify;
    char version;
    char timeZone;
    unsigned long epoch;
    unsigned long alarm;
  };

  HHSchema * get();
  void write(HHSchema &data);
}

#endif // _HEDGEHOG_PERSISTENCE_H
