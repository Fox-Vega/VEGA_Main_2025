#include <Arduino.h>
#include "Input.h"
#include "AIP.h"
#include "Process.h"

void setup() {
    general.setup();
    general.startup();
}

void loop() {
    if (myswitch.check_toggle() == 1 && general.get_mode() != 3) {
        if (general.get_mode() == 1) {
            gam.get_cord();
            ball.read();
            attack.attack_();
        } else if (general.get_mode() == 2) {
            // Serial.println("defence selected");
            // defence.defense_();
            // Serial.println("defence end");
        } else {
            test.test_();
        }
    } else {
        general.startup();
    }
}
