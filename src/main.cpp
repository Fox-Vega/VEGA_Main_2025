#include <Arduino.h>
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"
#include "Defense.h"

Timer timer_test;

void setup() {
    general.setup();
    general.startup();
}

void loop() {
    timer_test.reset();
    mypixel.clear();
    //Serial.println(">timer_test:"+String(timer_test.read_milli()));
    gam.get_cord();
    //Serial.println(">timer_test:"+String(timer_test.read_milli()));
    if (myswitch.check_toggle() == 1 && general.get_mode() != 3) {
        if (general.get_mode() == 1) {
            attack.attack_();
        } else if (general.get_mode() == 2) {
            defense.defense_();
        }
    } else if (general.get_mode() == 3) {
        test.test_();
    } else {
        general.startup();
    }
    //Serial.println(">timer_test:"+String(timer_test.read_milli()));
    mypixel.show();
    //Serial.println(">timer_test:"+String(timer_test.read_milli()));
}