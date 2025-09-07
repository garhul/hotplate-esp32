#pragma once

#include <Arduino.h>
#include "ADS1X15.h"
#define BTN_A 9
#define BTN_B 10
#define BTN_C 20
#define BTN_D 21

#define FAN_PIN 1
#define HEATER_PIN 0

#define I2C_SDA 2
#define I2C_SCL 3

#define SAFETY_TEMP_DIFF 100 //temp diff in raw adc reading
#define MAX_TEMP 250 // max target temp in C

namespace IO {
  struct btnState {
    bool a;
    bool b;
    bool c;
    bool d;
  };

  void init();
  void update();
  btnState getButtonState();
  void setTargetTemp(uint16_t temp);
  uint16_t getTemp();
  void fanOn();
  void fanOff();

}