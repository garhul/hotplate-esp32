#pragma once

#include <Arduino.h>
#include "state.hpp"
#include "io.h"
#include "display.h"
#include "profiles.h"

namespace Controller {
  extern SystemState::State* state;

  void init();
  void update();


  void menu();
  void serviceMenu();
  void serviceProfileSelection();
  // void profileSelectionControllerCycle(Model::SystemState& state);

}