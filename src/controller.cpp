#include "controller.h"

// Define the static member variable
namespace Controller {
  SystemState::State* state = nullptr;
  SystemState::Modes lastMode = SystemState::Modes::STARTUP;
  uint32_t lastModeChangeTime = millis();
  uint32_t lastUpdate = millis();
}

void Controller::init() {
  state = new SystemState::State();
  Serial.println("Controller initialized.");

  IO::init();
  Display::init();
  Profiles::loadProfiles();
}

void Controller::bindMenuButtons() {
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
        case SystemState::MenuItem::START_CYCLE:

          switchMode(SystemState::Modes::PROFILE_RUNNING);
          break;
        case SystemState::MenuItem::PROFILE_SELECT:
          switchMode(SystemState::Modes::PROFILE_SELECTION);
          break;
        case SystemState::MenuItem::SET_TEMP:
          switchMode(SystemState::Modes::SET_TEMP);
          break;
        case SystemState::MenuItem::SETTINGS:
          // switchMode( SystemState::Modes::SETTINGS);
          break;
      }
    }
  });
}

inline void unbindButtons() {
  IO::onBtnStateChange(nullptr);
}

void Controller::bindHeatingButtons() {
  IO::onBtnStateChange([](IO::btnState newState) {

    if (!newState.back) {
      switchMode(SystemState::Modes::MENU);
    }
  });
}

void Controller::bindProfileSelectionButtons() {
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
      switchMode(SystemState::Modes::MENU);
    }

    if (!newState.ok) {
      switchMode(SystemState::Modes::MENU);
    }
  });
}

void Controller::bindTempSetButtons() {
  IO::onBtnStateChange([](IO::btnState newState) {
     // Add a dead time out to prevent multiple triggers from a single button press
    if ((millis() - lastModeChangeTime) < 1000) {
      return;
    }
    state->buttons = newState;

    if (!newState.back) {
      switchMode(SystemState::Modes::MENU);
    }

    if (!newState.ok) {
      switchMode(SystemState::Modes::HEATING);
    }
  });
}

void Controller::switchMode(SystemState::Modes newMode) {
  unbindButtons();
  lastModeChangeTime = millis();
  lastMode = state->mode;
  state->mode = newMode;
}

void Controller::handleMode(bool modeChanged) {
  switch (state->mode) {
    case SystemState::Modes::STARTUP:
      IO::safeMode();
      if ((millis() - lastModeChangeTime) > 1000) {
        state->currentProfile = &Profiles::profiles[0];
        switchMode(SystemState::Modes::MENU);
      }
      break;

    case SystemState::Modes::MENU:
      IO::safeMode();

      if (modeChanged)
        bindMenuButtons();
      break;

    case SystemState::Modes::SET_TEMP:
      if (modeChanged) {
        IO::safeMode();
        bindTempSetButtons();
      }

      if (millis() - lastUpdate > 100) {
        lastUpdate = millis();
        if (!state->buttons.down) {
          if (state->tempTarget >= 5) {
            state->tempTarget -= 5;
          } else {
            state->tempTarget = 0;
          }
        } else if (!state->buttons.up) {
          state->tempTarget += 5;
          if (state->tempTarget > MAX_TEMP) state->tempTarget = MAX_TEMP;
        }
      }
      break;

    case SystemState::Modes::HEATING:
      if (modeChanged) {
        IO::safeMode();
        state->currentProfile = (state->currentProfile == nullptr) ? &Profiles::profiles[0] : state->currentProfile;
        bindProfileSelectionButtons();
        IO::setTargetTemp(state->tempTarget);
      }
      break;

    case SystemState::Modes::PROFILE_SELECTION:
      if (modeChanged) {
        state->currentProfile = (state->currentProfile == nullptr) ? &Profiles::profiles[0] : state->currentProfile;
        bindProfileSelectionButtons();
      }
      break;
  }
}

void Controller::update() {
  bool modeChanged = (state->mode != lastMode);

  if (modeChanged) {
    lastMode = state->mode;
  }

  IO::update();
  state->currentTemp = IO::getTemp();

  handleMode(modeChanged);
  Display::update(*state, modeChanged);

}