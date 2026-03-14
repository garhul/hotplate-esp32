#include <display.h>

namespace Display {
  TFT_eSPI _tft;
  uint16_t colors[] = { TFT_RED, TFT_GREEN, TFT_BLUE, TFT_CYAN, TFT_MAGENTA, TFT_YELLOW, TFT_WHITE, TFT_ORANGE, TFT_DARKGREY, TFT_DARKCYAN, TFT_BROWN, TFT_DARKGREEN };
}

inline void drawTitle(TFT_eSPI* tft, const char* title) {
  tft->setTextSize(2);
  tft->setTextFont(1);
  tft->setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft->setCursor((tft->width() - tft->textWidth(title)) / 2, 5);
  tft->print(title);
}

void Display::init() {
  _tft.init();
  _tft.setRotation(1);
  _tft.fillScreen(TFT_BLACK);
}

void Display::drawSplash() {
  _tft.setTextSize(4);
  _tft.setTextColor(TFT_DARKGREEN, TFT_BLACK);
  _tft.setCursor((_tft.width() - _tft.textWidth("ESP32")) / 2, _tft.height() / 2 - 40);
  _tft.print("ESP32");
  _tft.setCursor((_tft.width() - _tft.textWidth("HotPlate")) / 2, _tft.height() / 2);
  _tft.print("HotPlate");


  _tft.setTextSize(2);
  _tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  const char* copy = "(c) Garhul";
  _tft.setCursor((_tft.width() - _tft.textWidth(copy)) / 2, _tft.height() - 20);
  _tft.print(copy);

  _tft.setTextSize(1);
  _tft.setCursor(200, _tft.height() / 2 + 34);
  _tft.print(FIRMWARE_VERSION);
}

void Display::drawMenu(SystemState::State st, bool firstFrame) {
/* Menu
 *  > Run cycle [ profile name]  | > Select profile
 *  > Set Temperature            | > PID tuning
 * */
  if (firstFrame) {
    drawTitle(&_tft, "::MENU::");
  }

  const char* items[] = { "Run cycle", "Set Profile","Settings","Set Temp" };

  _tft.setTextSize(2);
  for (int i = 0; i < static_cast<int>(SystemState::MenuItem::COUNT); i++) {
    bool selected = (i == static_cast<int>(st.selectedMenuItem));

    int y = 45 + i * 24;

    _tft.setTextColor(selected ? TFT_DARKGREEN : TFT_DARKGREY, TFT_BLACK);
    _tft.setCursor(8, y);
    _tft.print(items[i]);

    if (st.currentProfile != nullptr && i == static_cast<int>(SystemState::MenuItem::START_CYCLE)) {
      _tft.setCursor(_tft.textWidth(items[i]) + 5, y);
      _tft.print(String(" [") + String(st.currentProfile->name) + String("]"));
    }

  }
}

inline void drawProfile(TFT_eSPI* tft, Profiles::Profile* profile) {
  uint16_t color = tft->color565(90, 60, 0);
  uint16_t gridColor = tft->color565(10, 10, 10);
  uint16_t gridLabelColor = tft->color565(40, 40, 40);
  uint16_t maxTemp = 250;
  uint16_t y_margin = 30;
  uint16_t x_margin = 30;

  // Title
  drawTitle(tft, profile->name);

  //Draw the grid for Y
  tft->drawLine(x_margin, y_margin, x_margin, tft->height() - y_margin, gridColor);
  tft->drawLine(x_margin, tft->height() - y_margin, tft->width() - 3, tft->height() - y_margin, gridColor);

  for (uint16_t temp = 50; temp <= maxTemp; temp += 50) {
    int y = tft->height() - (temp * (tft->height() - y_margin) / maxTemp);
    tft->drawLine(x_margin, y, tft->width(), y, gridColor);
    tft->setCursor(5, y - 5);
    tft->setTextColor(gridLabelColor, TFT_BLACK);
    tft->setTextSize(1);
    tft->print(temp);
  }

  //iterate through profile phases and draw them
  Profiles::ProfilePhase* phase = profile->start;

  float pixelsPerSecond = (float)(tft->width() - x_margin) / (float)profile->durationSeconds;
  float pixelsPerDegree = (float)(tft->height() - y_margin * 2) / (float)maxTemp;

  int x = x_margin;
  while (phase != nullptr) {
    uint16_t t0 = (phase->prev != nullptr) ? phase->prev->targetTemp : 0;
    uint16_t w = ceil(phase->durationSeconds * pixelsPerSecond);

    uint16_t y0 = tft->height() - (((float)t0 * pixelsPerDegree) + y_margin);
    uint16_t y1 = tft->height() - (((float)phase->targetTemp * pixelsPerDegree) + y_margin);

    tft->drawLine(x, y0, x + w, y1, color);

    //draw phase time
    tft->setTextSize(1);
    tft->setTextColor(gridLabelColor, TFT_BLACK);
    int timeX = x + (w / 2) - tft->textWidth(String(phase->durationSeconds) + "s") / 2;
    tft->setCursor(timeX, tft->height() - y_margin);
    tft->print(String(phase->durationSeconds) + "s");
    tft->drawLine(x, tft->height() - y_margin, x, y_margin, gridColor);

    // draw text for phase 
    // tft->setTextSize(1);
    // tft->setTextColor(gridLabelColor, TFT_BLACK);
    // int textX = (x + w / 2) - tft->textWidth(phase->name) / 2;
    // tft->setCursor(textX, y0 + (y1 / 2));
    // tft->print(phase->name);

    x += w;
    phase = phase->next;

    if (phase == nullptr) {
      w = ceil(profile->durationSeconds * pixelsPerSecond);
      tft->drawLine(x, y1, x + w, y1, color);
    }
  }


  // tft->drawLine(30, tft->height() - 30, tft->width(), tft->height() - 30, color);



}

void Display::drawProfileSelection(SystemState::State st, bool firstFrame) {
  static Profiles::Profile* lastProfile = st.currentProfile;
  bool profileChanged = (st.currentProfile != lastProfile);

  if (firstFrame || profileChanged) {
    lastProfile = st.currentProfile;

    if (st.currentProfile == nullptr) {
      st.currentProfile = &Profiles::profiles[0];
    };

    _tft.fillScreen(TFT_BLACK);
    drawProfile(&_tft, st.currentProfile);
  }
}

void Display::drawSetTemp(SystemState::State st, bool firstFrame) {
  static uint16_t lastTempTarget = st.tempTarget;
  static uint16_t lastTempCurrent = st.currentTemp;

  if (firstFrame) {
    if (st.mode == SystemState::Modes::SET_TEMP) {
      drawTitle(&_tft, "::SET TEMP::");
    } else if (st.mode == SystemState::Modes::HEATING) {
      drawTitle(&_tft, "::HEATING::");
    }
  }

  if (st.tempTarget != lastTempTarget || firstFrame) {

    if (st.mode == SystemState::Modes::SET_TEMP) {
      _tft.setTextColor(TFT_CYAN, TFT_BLACK);
    } else if (st.mode == SystemState::Modes::HEATING) {
      _tft.setTextColor(_tft.color565(80, 50, 50), TFT_BLACK);
    }

    _tft.setTextFont(1);
    _tft.setTextSize(2);
    _tft.setCursor(0, 120);
    _tft.printf("Target:");

    _tft.setTextFont(7);
    _tft.setTextSize(1);
    _tft.setCursor(80, 120);
    _tft.printf("%03d", st.tempTarget);
  }

  if (st.currentTemp != lastTempCurrent || firstFrame) {
    if (st.mode == SystemState::Modes::SET_TEMP) {
      _tft.setTextColor(_tft.color565(50, 50, 50), TFT_BLACK);
    } else if (st.mode == SystemState::Modes::HEATING) {
      _tft.setTextColor(TFT_RED, TFT_BLACK);
    }

    _tft.setTextFont(1);
    _tft.setTextSize(2);
    _tft.setCursor(0, 60);
    _tft.printf("Temp:");

    _tft.setTextFont(7);
    _tft.setTextSize(1);
    _tft.setCursor(80, 60);
    _tft.printf("%04d", st.currentTemp);
  }
}

void Display::update(SystemState::State st, bool modeChanged) {
  if (modeChanged) {
    _tft.fillScreen(TFT_BLACK);
  }

  switch (st.mode) {
    case SystemState::Modes::STARTUP:
      Display::drawSplash();
      break;
    case SystemState::Modes::MENU:
      Display::drawMenu(st, modeChanged);
      break;
    case SystemState::Modes::PROFILE_SELECTION:
      Display::drawProfileSelection(st, modeChanged);
      break;
    case SystemState::Modes::SET_TEMP:
    case SystemState::Modes::HEATING:
      Display::drawSetTemp(st, modeChanged);
      break;
  }
}