#include <Arduino.h>
#include "Input.h"
#include "AIP.h"


void setup() {
  general.setup();
}

void loop() {
  gam.get_cord();
  Serial.print(">pos_x:");
  Serial.println(gam.get_x());
  Serial.print(">pos_y:");
  Serial.println(gam.get_y());
}