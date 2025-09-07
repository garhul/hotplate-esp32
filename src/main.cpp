#include <Arduino.h>

//configured in user_setup.h under its own folder
#include "io.h"
#include "screens.h"



void setup() {
  Serial.begin(460800);
  printf("Setup started.");
  IO::init();
  MODES::init();
  // Initialize I2C with specified SDA and SCL pins and frequency
  Serial.println("Setup complete.");
}




void loop() {
  IO::update();
  MODES::update(IO::getButtonState());
  delay(10);
}
