#include <Arduino.h>
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"
#include "Defense.h"

Timer timer_test;
static int startcord_first = 0;

void setup() {
    general.setup();
    defense.setup();
    startcord_first = general.startup();
}

void loop() {
    line.read();
    ball.read();
    mypixel.clear();
    gam.read_azimuth();
    if (myswitch.check_toggle() == 1 ) {
        if (general.get_mode() == 1) {
            attack.attack_();
        } else if (general.get_mode() == 2) {
            defense.defense_(startcord_first);
        } else if (general.get_mode() == 3) {
            test.test_();
        }
        startcord_first = 0;
    } else {
        mypixel.use_pixel(true);
        Serial.println(defense.defense_hadling_timeget());
        Serial.println("-----");
        startcord_first = general.startup();
        mypixel.use_pixel(false);
        defense.reset();
    }
    mypixel.show();
}