#include <Arduino.h>
#include "Input.h"
#include "AIP.h"
#include "Process.h"


void setup() {
    general.setup();
    general.startup();
}

void loop() {
    gam.get_cord();
    if (general.get_mode() == 1) {
        // attack.attack_();
    } else if (general.get_mode() == 2) {
        defence.defence();
    } else {
        //test
    }
}
