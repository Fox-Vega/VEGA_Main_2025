#include <Arduino.h>
#include "Input.h"
#include "AIP.h"


void setup() {
  general.setup();
}

void loop() {
  gam.get_cord();
  int Input = Serial.read();
  switch(Input) {
    case 1:
        gam.cord_reset();
    case 2:
        gam.restart();
    case 3:
        gam.dir_reset();
  }
}