#include "screens.h"


namespace MODES {
  TFT_eSPI tft = TFT_eSPI(); // Create object "tft"
  uint16_t colors[] = { TFT_RED, TFT_GREEN, TFT_BLUE, TFT_CYAN, TFT_MAGENTA, TFT_YELLOW, TFT_WHITE, TFT_ORANGE, TFT_DARKGREY, TFT_COLMOD, TFT_CASET, TFT_DARKGREEN };
  Screen currentScreen = Screen::MAIN;

  inline uint16_t getColor(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 11) | (g << 5) | b;
  }

  void showSplashScreen() {
    tft.fillScreen(0);
    tft.drawRect(0, 0, tft.width(), tft.height(), TFT_MAGENTA);
    tft.setTextColor(TFT_WHITE);

    tft.setCursor(20, 60);
    tft.setTextFont(3);
    tft.print("Garhul's \n");
    tft.println("  Hot Plate");

    tft.setTextFont(2);
    tft.setTextColor(TFT_YELLOW);
    tft.setCursor(10, 216);
    tft.print("V 0.0.1");
    tft.setTextColor(TFT_CYAN);
    tft.setCursor(116, 216);
    tft.print("(c) Garhul's 2024");
    delay(2000);
    tft.fillScreen(TFT_BLACK);
  }

  void showSettingsScreen() {
  //  tft.setTextColor(TFT_WHITE);
    tft.setTextFont(6);
    tft.print("230 C");
  }

  void showMainScreen() {
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(20, 10);
    tft.print("Main Screen");
    tft.setTextSize(1);
    tft.setCursor(30, 40);
    tft.print("Press A for Settings");
    tft.setCursor(30, 60);
    tft.print("Press B for Calibration");
    tft.setCursor(30, 80);
    tft.print("Press C for Error");

    // Code to display the main screen
  }

  void init() {
    tft.begin();
    tft.setRotation(1); // Set the rotation of the display
    tft.fillScreen(0);
    showSplashScreen();
  }

  void clearScreen() {
    tft.fillScreen(TFT_BLACK);
  }


  // Bootstrapps each mode on change
  void switchMode(Screen newScreen) {
    switch (newScreen) {
    case Screen::MAIN:
      /**from main allowable transitions are
       * PROFILE_SELECTION, PROFILE_RUNNING
       * ERROR, HEATING  ?
       */

      IO.fanOff();
      IO.heaterOff();
      break;

    case Screen::PROFILE_RUNNING:
      IO.fanOff();
      break;

    case Screen::HEATING:
      IO.fanOff();
      break;

    case Screen::PROFILE_SELECTION:
      IO.fanOff();
      IO.heaterOff();
      break;

    case Screen::COOLING:
      IO.fanOn();
      IO.heaterOff();
      break;
    }

    currentScreen = newScreen;
  }



  void drawProfile() {
    
    
    
    uint16_t soakTime = 180;
    uint16_t soakTemp = 150;


    uint16_t heatingTime = 120;
    uint16_t coolingTime = 60;
    uint16_t 
  }


  void update(IO::btnState btnState) {
    static Screen currentScreen = Screen::MAIN;
    // Update the screen based on the current screen and button state
    switch (currentScreen) {
    case Screen::SPLASH:
      showSplashScreen();
      break;
    case Screen::MAIN:
      if (!btnState.a) {
        currentScreen = Screen::SETTINGS;
        clearScreen();
      }
      showMainScreen();
      break;
    case Screen::SETTINGS:
      if (!btnState.a) {
        currentScreen = Screen::MAIN;
        clearScreen();
      }
      showSettingsScreen();
      break;
    case Screen::CALIBRATION:
      // Add calibration screen logic here
      break;
    case Screen::ERROR:
      // Add error handling logic here
      break;
    }
  }

  void showScreen(Screen screen) {
    switch (screen) {
    case Screen::SPLASH:
      showSplashScreen();
      break;
    case Screen::MAIN:
      showMainScreen();
      break;
    case Screen::SETTINGS:
      break;
    }
  }

} // namespace SCREENS