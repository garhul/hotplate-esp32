#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h> // Include the graphics library (this includes the sprite functions)
#include "SPI.h"

#include "io.h"

namespace MODES {
  struct profile {
    uint16_t preHeatTime;
    uint16_t preHeatTemp;
    uint16_t soakTime;
    uint16_t soakTemp;
    uint16_t reflowTime;
    uint16_t reflowTemp;
    uint16_t reflowHoldTime;
    uint16_t coolingTime;
  };

  enum class Screen {
    MAIN,
    PROFILE_RUNNING,
    HEATING,
    COOLING,
    PROFILE_SELECTION,
    ERROR
  };

  void init();
  void update(IO::btnState btnState);

}