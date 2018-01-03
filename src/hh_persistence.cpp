#include "hh_persistence.h"
// #include "Arduino.h" //debug

HHPersistence::HHSchema * HHPersistence::get() {
  int addr = 0;
  HHSchema *data = new HHSchema();

  EEPROM.begin(sizeof(HHSchema));

  data->verify = EEPROM.read(addr++);
  data->version = EEPROM.read(addr++);

  // Serial.printf("READ verify %d [%c], version %d [%c]", data->verify, data->verify, data->version, data->version); //debug

  if (data->verify != 'H' || data->version != 'a')
    return NULL;

  data->timeZone = EEPROM.read(addr++);
  data->epoch = EEPROM.read(addr++)
      | (EEPROM.read(addr++) << 8)
      | (EEPROM.read(addr++) << 16)
      | (EEPROM.read(addr++) << 24);
  data->alarm = EEPROM.read(addr++)
      | (EEPROM.read(addr++) << 8)
      | (EEPROM.read(addr++) << 16)
      | (EEPROM.read(addr++) << 24);

  EEPROM.end();

  return data;

  // return NULL; //debug
}

bool HHPersistence::write(HHSchema& data) {
  int addr = 0;

  // Serial.printf("WRITE verify %d [%c], version %d [%c]", data.verify, data.verify, data.version, data.version); //debug

  EEPROM.begin(sizeof(HHSchema));

  EEPROM.write(addr++, data.verify);
  EEPROM.write(addr++, data.version);
  EEPROM.write(addr++, data.timeZone);
  EEPROM.write(addr++, data.epoch & 0xFF);
  EEPROM.write(addr++, (data.epoch >> 8) & 0xFF);
  EEPROM.write(addr++, (data.epoch >> 16) & 0xFF);
  EEPROM.write(addr++, (data.epoch >> 24) & 0xFF);
  EEPROM.write(addr++, data.alarm & 0xFF);
  EEPROM.write(addr++, (data.alarm >> 8) & 0xFF);
  EEPROM.write(addr++, (data.alarm >> 16) & 0xFF);
  EEPROM.write(addr++, (data.alarm >> 24) & 0xFF);

  return EEPROM.commit();

  // return 1; //debug
}