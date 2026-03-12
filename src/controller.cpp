#include "controller.h"

// Define the static member variable
namespace Controller {
  SystemState::State* state = nullptr;
}

void Controller::init() {
  IO::init();

  Display::init();
  state = new SystemState::State();
  Serial.println("Controller initialized.");
  Profiles::loadProfiles();
}

void Controller::serviceMenu() {
  IO::fanOff();
  IO::heaterOff();

  IO::onBtnStateChange([](IO::btnState newState) {
    Serial.printf("Button state changed: up=%d down=%d ok=%d back=%d\n", newState.up, newState.down, newState.ok, newState.back);
    uint8_t selectedItem = static_cast<uint8_t>(state->selectedMenuItem);

    if (!newState.down) {
      state->selectedMenuItem = static_cast<SystemState::MenuItem>(++selectedItem % static_cast<int>(SystemState::MenuItem::COUNT));
    } else if (!newState.up) {
      selectedItem = (selectedItem == 0) ? static_cast<uint8_t>(SystemState::MenuItem::COUNT) : selectedItem;
      state->selectedMenuItem = static_cast<SystemState::MenuItem>(selectedItem - 1);
    }

    if (!newState.ok) {
      Serial.printf("Menu item changed: %d\n", static_cast<int>(state->selectedMenuItem));
      switch (state->selectedMenuItem) {
      //   case SystemState::MenuItem::START_CYCLE:
      //     // state->mode = SystemState::Modes::PROFILE_RUNNING;
      //     break;
        case SystemState::MenuItem::PROFILE_SELECT:
          state->mode = SystemState::Modes::PROFILE_SELECTION;
          break;
      //   case SystemState::MenuItem::SET_TEMP:
      //     // state->mode = SystemState::Modes::TEMP_SETTING;
      //     break;
      //   case SystemState::MenuItem::SETTINGS:
      //     // state->mode = SystemState::Modes::SETTINGS;
      //     break;
      }
    }
  });
}

void Controller::serviceProfileSelection() {
  IO::fanOff();
  IO::heaterOff();

  state->currentProfile = (state->currentProfile == nullptr) ? &Profiles::profiles[0] : state->currentProfile;

  IO::onBtnStateChange([](IO::btnState newState) {
    Serial.printf("profiles count: %d, current index: %d\n", Profiles::profileCount, Profiles::currentProfileIndex);
    state->previousProfile = state->currentProfile;

    if (!newState.down) {
      Profiles::currentProfileIndex = ++Profiles::currentProfileIndex % (Profiles::profileCount + 1);
      state->currentProfile = &Profiles::profiles[Profiles::currentProfileIndex];
      Serial.printf("selected item: %d\n", Profiles::currentProfileIndex);
    } else if (!newState.up) {
      Profiles::currentProfileIndex = (Profiles::currentProfileIndex == 0) ? Profiles::profileCount : Profiles::currentProfileIndex - 1;
      state->currentProfile = &Profiles::profiles[Profiles::currentProfileIndex];
      Serial.printf("selected item: %d\n", Profiles::currentProfileIndex);
    }

    if (!newState.back) {
      state->currentProfile = state->previousProfile;
      state->mode = SystemState::Modes::MENU;
    }

    if (!newState.ok) {
      state->mode = SystemState::Modes::MENU;
    }
  });
}

void Controller::update() {
  static SystemState::Modes lastMode = state->mode;
  state->stageElapsedMs = millis() - state->stageElapsedMs;
  bool modeChanged = (state->mode != lastMode);
  Display::update(*state);

  if (modeChanged) {
    state->stageElapsedMs = millis();
    lastMode = state->mode;
  }

  switch (state->mode) {
    case SystemState::Modes::STARTUP:
      IO::fanOff();
      IO::heaterOff();

      // Show splash for 3 seconds
      if (state->stageElapsedMs > 3000) {
        state->currentProfile = &Profiles::profiles[0];
        state->mode = SystemState::Modes::MENU;
      }
      // Handle main screen logic
      break;
    case SystemState::Modes::MENU:
      if (modeChanged) {
        serviceMenu();
      }
      break;
    case SystemState::Modes::TEMP_SETTING:
  // Handle temperature setting logic
      break;
    case SystemState::Modes::PID_TUNING:
  // Handle PID tuning logic
      break;
    case SystemState::Modes::PROFILE_SELECTION:

      if (modeChanged) {

        serviceProfileSelection();
      }

      break;

  // Handle profile selection logic
      break;
    case SystemState::Modes::PROFILE_RUNNING:
  // Handle profile running logic
      break;
    case SystemState::Modes::PAUSED:
  // Handle paused state logic
      break;
    case SystemState::Modes::HEATING:
  // Handle heating mode logic
      break;
    case SystemState::Modes::COOLING:
  // Handle cooling mode logic
      break;
    case SystemState::Modes::SETTINGS:
  // Handle settings logic
      break;
    case SystemState::Modes::ERROR:
  // Handle error state logic
      break;
  }

}

