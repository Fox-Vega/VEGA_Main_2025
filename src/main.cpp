#include <Arduino.h>
#include "Input.h"
#include "AIP.h"
#include "Process.h"


void setup() {
    general.setup();
    general.startup();
}

void loop() {
    if (general.get_mode() == 1) {
        gam.get_cord();
        ball.read();
        // attack.attack_();
    } else if (general.get_mode() == 2) {
        defence.defence();
    } else {
        //test
    }
}
