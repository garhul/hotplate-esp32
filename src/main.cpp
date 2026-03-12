#include <Arduino.h>
//NOTE TFT is configured in user_setup.h under its own folder
#include "controller.h"


void setup() {
  Serial.begin(460800);
  delay(1000);

  Controller::init();
  Serial.println("Setup complete.");

  Profiles::Profile profiles[10];
  Profiles::getProfilesList(profiles, Profiles::profileCount);
}


void loop() {
  Controller::update();
  delay(10);
}
