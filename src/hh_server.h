#ifndef _HEDGEHOG_SERVER_H
#define _HEDGEHOG_SERVER_H

#include "ESP8266WebServer.h"
#include "hh_rom.h"

namespace HHServer {
  void beginServer(HHROM::HHSchema *data);
  void handleClient();
}


#endif // _HEDGEHOG_SERVER_H