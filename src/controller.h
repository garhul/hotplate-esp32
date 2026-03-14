#pragma once

#include <Arduino.h>
#include "state.hpp"
#include "io.h"
#include "display.h"
#include "profiles.h"

namespace Controller {
  extern SystemState::State* state;
  extern uint32_t lastUpdate;
  extern uint32_t lastModeChangeTime;
  extern SystemState::Modes lastMode; //track mode changes for button handler attachment and display updates

  void init();
  void update();
  inline void handleMode(bool modeChanged);
  inline void attachUserInputHandlers();
  inline void switchMode(SystemState::Modes newMode);

  void startupHandler();
  void bindMenuButtons();
  void bindProfileSelectionButtons();
  void bindTempSetButtons();
  void bindHeatingButtons();
  /// maybe..
  void pidTuningHandler();
  void profileRunningHandler();
  void pausedHandler();
  // void profileSelectionControllerCycle(Model::SystemState& state);

}