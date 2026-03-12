#pragma once

#include <Arduino.h>
#include "io.h"
#include "profiles.h"

/**
 * This keeps the application running state, from which the controller infers data and the display renders the UI
 *
 * There are Stages for each profile and modes of operation with different views.
 *
 * Menu
 *  > Run cycle [ profile name]  | > Select profile
 *  > Set Temperature            | > PID tuning
 *
 * Run profile view:
 *
 * > Show current graph with target temperatures and current temp curve
 * > Show button to pause/resume & cancel
 * > Show current stage (preheat, soak, reflow, cooldown)
 * > Show elapsed time in current stage and total elapsed time
 * > show target temp and current temp
 *
 * Set temperature view:
 * > Show current temp and target temp
 * > Show buttons to increase/decrease target temp, and start manual heating/cooling
 * > Show button to return to menu
 *
 * PID tuning view:
 * > Show current PID values and allow adjusting them with buttons, show back button, do self calibration ?
 *
 * Profile selection view:
 * > Next ->  Previous
 * > OK [selects]  ->  Back [cancels]
 *
 *
 *
 */


namespace SystemState {

  enum class Modes {
    STARTUP, //shows splash screen, then moves to menu
    MENU, // Shows operation modes options like "Select profile" "PID tuning" "Run profile" "Set Temperature"
    TEMP_SETTING, // Set target temperature, show controls Up Down, start, back to menu
    PID_TUNING,
    PROFILE_SELECTION,
    PROFILE_RUNNING,
    PAUSED, // Pause current profile, keep displaying current cycle info
    HEATING, // Manual mode: turn on heater, keep fan off
    COOLING, // Manual mode: turn on fan, keep heater off
    SETTINGS,
    ERROR
  };

  enum class MenuItem { //Eventually move this to its own file
    START_CYCLE,
    PROFILE_SELECT,
    SETTINGS,
    SET_TEMP,
    COUNT
  };

  class State {
    public:
    IO::btnState buttons{ false, false, false, false };
    Modes mode{ Modes::STARTUP };
    MenuItem selectedMenuItem{ MenuItem::START_CYCLE };

    float kp{ 1.0f };
    float ki{ 0.0f };
    float kd{ 0.0f };

    uint16_t tempCurrent{ 0 };
    uint16_t tempTarget{ 0 };


    Profiles::Profile* currentProfile{ nullptr };
    Profiles::Profile* previousProfile{ nullptr };
    Profiles::ProfilePhase* currentPhase{ nullptr };

    // Profile profile{};
    uint32_t stageElapsedMs{ 0 };

    bool hasError{ false };
    String errorMessage{ "" };
  };
}