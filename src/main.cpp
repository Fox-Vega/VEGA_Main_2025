#include <Arduino.h>
#include "Input.h"
#include "AIP.h"


void setup() {
  general.setup();
}

void loop() {
  gyro.get_cord();
  Serial.print(">pos_x:");
  Serial.println(gyro.get_x());
  Serial.print(">pos_y:");
  Serial.println(gyro.get_y());
}