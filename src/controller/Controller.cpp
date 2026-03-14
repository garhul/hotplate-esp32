// #include "controller/Controller.h"
// #include "io.h"

// namespace Controller {
//   namespace {
//     IO::btnState prevButtons{ true, true, true, true };

//     bool isPressed(const IO::btnState& buttons, bool IO::btnState::* member) {
//       return !(buttons.*member);
//     }

//     bool risingEdge(const IO::btnState& buttons, bool IO::btnState::* member) {
//       bool prev = !(prevButtons.*member);
//       bool curr = !(buttons.*member);
//       return (!prev && curr);
//     }

//     void advanceProfileStage(Model::SystemState& state) {
//       state.stageElapsedMs = 0;
//       switch (state.profileStage) {
//         case Model::ProfileStage::IDLE:
//           state.profileStage = Model::ProfileStage::PREHEAT;
//           state.tempTarget = state.profile.preHeatTemp;
//           break;
//         case Model::ProfileStage::PREHEAT:
//           state.profileStage = Model::ProfileStage::SOAK;
//           state.tempTarget = state.profile.soakTemp;
//           break;
//         case Model::ProfileStage::SOAK:
//           state.profileStage = Model::ProfileStage::REFLOW;
//           state.tempTarget = state.profile.reflowTemp;
//           break;
//         case Model::ProfileStage::REFLOW:
//           state.profileStage = Model::ProfileStage::COOLDOWN;
//           state.tempTarget = 0;
//           break;
//         case Model::ProfileStage::COOLDOWN:
//           state.profileStage = Model::ProfileStage::COMPLETE;
//           state.tempTarget = 0;
//           break;
//         case Model::ProfileStage::COMPLETE:
//           state.profileStage = Model::ProfileStage::IDLE;
//           state.tempTarget = 0;
//           break;
//       }
//     }

//     void updateProfile(Model::SystemState& state, uint32_t dtMs) {
//       if (state.profileStage == Model::ProfileStage::IDLE) {
//         return;
//       }

//       state.stageElapsedMs += dtMs;

//       switch (state.profileStage) {
//         case Model::ProfileStage::PREHEAT:
//           if (state.stageElapsedMs >= state.profile.preHeatTimeMs) {
//             advanceProfileStage(state);
//           }
//           break;
//         case Model::ProfileStage::SOAK:
//           if (state.stageElapsedMs >= state.profile.soakTimeMs) {
//             advanceProfileStage(state);
//           }
//           break;
//         case Model::ProfileStage::REFLOW:
//           if (state.stageElapsedMs >= state.profile.reflowTimeMs + state.profile.reflowHoldTimeMs) {
//             advanceProfileStage(state);
//           }
//           break;
//         case Model::ProfileStage::COOLDOWN:
//           if (state.stageElapsedMs >= state.profile.coolingTimeMs) {
//             advanceProfileStage(state);
//           }
//           break;
//         case Model::ProfileStage::COMPLETE:
//         case Model::ProfileStage::IDLE:
//           break;
//       }
//     }

//     void applyHeaterControl(const Model::SystemState& state, uint32_t nowMs) {
//       static uint32_t windowStartMs = 0;
//       static float integral = 0.0f;
//       static float lastError = 0.0f;

//       const float kp = 6.0f;
//       const float ki = 0.02f;
//       const float kd = 2.0f;

//       const float error = static_cast<float>(state.tempTarget) - static_cast<float>(state.currentTemp);
//       integral += error;
//       const float derivative = error - lastError;
//       lastError = error;

//       float output = kp * error + ki * integral + kd * derivative;
//       if (output < 0.0f) {
//         output = 0.0f;
//       }
//       if (output > 255.0f) {
//         output = 255.0f;
//       }

//       const uint32_t windowSizeMs = 1000;
//       if (nowMs - windowStartMs >= windowSizeMs) {
//         windowStartMs = nowMs;
//       }

//       const uint32_t onTimeMs = static_cast<uint32_t>((output / 255.0f) * windowSizeMs);
//       if ((nowMs - windowStartMs) < onTimeMs) {
//         IO::heaterOn();
//       } else {
//         IO::heaterOff();
//       }
//     }
//   }

//   void init(Model::SystemState& state) {
//     state.tempTarget = 200;
//     state.screen = Model::Screen::MAIN;
//     state.profileStage = Model::ProfileStage::IDLE;
//     state.stageElapsedMs = 0;
//   }

//   void update(Model::SystemState& state, uint32_t dtMs) {

//     state.buttons = IO::getButtonState();
//     state.currentTemp = IO::getTemp();

//     state.error = IO::getErrorFlag();
//     state.errorMessage = IO::getErrorMessage();

//     if (state.error) {
//       state.screen = Model::Screen::ERROR;
//       IO::heaterOff();
//       IO::fanOn();
//     } else {
//       if (risingEdge(state.buttons, &IO::btnState::a)) {
//         if (state.screen == Model::Screen::MAIN) {
//           state.screen = Model::Screen::SETTINGS;
//         } else if (state.screen == Model::Screen::SETTINGS) {
//           state.screen = Model::Screen::MAIN;
//         }
//       }

//       if (risingEdge(state.buttons, &IO::btnState::b)) {
//         state.screen = Model::Screen::PROFILE_SELECTION;
//       }

//       if (risingEdge(state.buttons, &IO::btnState::c)) {
//         state.screen = Model::Screen::PROFILE_RUNNING;
//         state.profileStage = Model::ProfileStage::PREHEAT;
//         state.stageElapsedMs = 0;
//         state.tempTarget = state.profile.preHeatTemp;
//       }

//       if (state.screen == Model::Screen::PROFILE_RUNNING) {
//         updateProfile(state, dtMs);
//       }

//       if (state.screen == Model::Screen::HEATING || state.screen == Model::Screen::PROFILE_RUNNING) {
//         IO::fanOff();
//         applyHeaterControl(state, millis());
//       } else if (state.screen == Model::Screen::COOLING) {
//         IO::heaterOff();
//         IO::fanOn();
//       } else {
//         IO::heaterOff();
//         IO::fanOff();
//       }
//     }

//     prevButtons = state.buttons;
//   }
// }
