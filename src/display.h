#pragma once

#include <Arduino.h>
#include <state.hpp>
#include <io.h>
#include <TFT_eSPI.h>
#include "version.h"

// #define TFT_DC    GPIO_NUM_7
// #define TFT_RST   GPIO_NUM_8
// #define TFT_MOSI  GPIO_NUM_5
// #define TFT_SCLK  GPIO_NUM_6

namespace Display {
  extern TFT_eSPI _tft;
  extern uint16_t colors[];

  enum class Screen {
    SPLASH,
    SETTINGS,
    MAIN,
    PROFILE_RUNNING,
    HEATING,
    COOLING,
    PROFILE_SELECTION,
    ERROR
  };




  void init();
  void update(SystemState::State  st);

  void drawSplash();
  void drawMenu(SystemState::State st, bool firstFrame);
  void drawProfileSelection(SystemState::State st, bool firstFrame);
  // void drawCurrentCycle(State& st);
  // void drawPIDCalibration(State& st);

};