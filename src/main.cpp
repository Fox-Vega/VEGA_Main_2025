#include <Arduino.h>
#include "Input.h"
#include "AIP.h"


void setup() {
    general.setup();
}

void loop() {
    gam.get_cord();
    if (general.get_mode() == 1) {
        attack.attack_();
    } else if (general.get_mode() == 2) {
        //defence
    } else {
        //test
    }
}
