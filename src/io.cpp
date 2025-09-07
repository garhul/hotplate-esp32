#include "io.h"

namespace IO {
  ADS1115 ADS(0x48);
  btnState currentBtnState = { false, false, false, false };
  int16_t targetTemp = 0;
  int16_t currentTemp = 0;

  bool errorFlag = false;
  String errorMessage = "";

  IRAM_ATTR void btn_isr() {
    static uint32_t last_isr_time = millis();

    if (millis() - last_isr_time < 50) { // Debounce time
      return;
    }

    last_isr_time = millis();

    currentBtnState.a = digitalRead(BTN_A);
    currentBtnState.b = digitalRead(BTN_B);
    currentBtnState.c = digitalRead(BTN_C);
    currentBtnState.d = digitalRead(BTN_D);
  }


  void init() {
    pinMode(BTN_A, INPUT_PULLUP);
    pinMode(BTN_B, INPUT_PULLUP);
    pinMode(BTN_C, INPUT_PULLUP);
    pinMode(BTN_D, INPUT_PULLUP);

    pinMode(FAN_PIN, OUTPUT);
    pinMode(HEATER_PIN, OUTPUT);

    digitalWrite(FAN_PIN, LOW);
    digitalWrite(HEATER_PIN, LOW);


    Wire.begin(I2C_SDA, I2C_SCL);
    ADS.begin();
    ADS.setGain(0); // Set gain to 0 for full range
    ADS.setMode(1); // Set to single-shot mode

    attachInterrupt(BTN_A, btn_isr, CHANGE);
    attachInterrupt(BTN_B, btn_isr, CHANGE);
    attachInterrupt(BTN_C, btn_isr, CHANGE);
    attachInterrupt(BTN_D, btn_isr, CHANGE);

  }


  inline void _updateHeater() {

  }

  inline void _readTemp() {
    int16_t t0 = ADS.readADC(2);
    int16_t t1 = ADS.readADC(3);

    if (abs(t0 - t1) > SAFETY_TEMP_DIFF) {
      errorFlag = true;
      errorMessage = "Temperature difference too high!";
      Serial.println(errorMessage);
      return;
    }
  }

  void update() {
    // Update button states or other IO operations
    _readTemp();
    _updateHeater();

  }


  void setTargetTemp(uint16_t temp) {
    if (temp > MAX_TEMP) {
      errorFlag = true;
      errorMessage = "Target temperature too high!";
      Serial.println(errorMessage);
      return;
    }

    targetTemp = temp;
  }

  uint16_t getTemp() {
    return currentTemp;
  }

  void fanOn(uint8_t speed) {
    analogWrite(FAN_PIN, speed);
  }

  void fanOff() {
    digitalWrite(FAN_PIN, LOW);
  }

  btnState getButtonState() {
    return currentBtnState;
  }


}