#pragma once

#include <Arduino.h>
#include "ADS1X15.h"

#define BTN_A 4
#define BTN_B 1
#define BTN_C 10
#define BTN_D 9

#define FAN_PIN 20
#define HEATER_PIN 21

#define I2C_SDA 2
#define I2C_SCL 3

#define SAFETY_TEMP_DIFF 100 //temp diff in raw adc reading
#define MAX_TEMP 250 // max target temp in C

namespace IO {
  struct btnState {
    bool down;
    bool back;
    bool ok;
    bool up;
    uint8_t get() {
      return (down ? 0 : 1) | (back ? 0 : 2) | (ok ? 0 : 4) | (up ? 0 : 8);
    }
  };

  void init();
  void update();
  void _updatePid();
  void setTargetTemp(uint16_t temp);

  btnState getButtonState();

  void onBtnStateChange(void (*callback)(btnState newState));

  uint16_t getTemp();

  void safeMode();
  void fanOn();
  void fanOff();

  void heaterOn();
  void heaterOff();

}