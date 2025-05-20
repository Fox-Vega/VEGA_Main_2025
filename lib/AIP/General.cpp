#include "General.h"
#include "Input.h"
#include "Output.h"


void General::setup() {
    Serial.begin(9600);
    ball.setup();
    gam.setup();
    // line.setup();
    mymotor.setup();
    mypixel.setup();
    mybuzzer.setup();
    myswitch.setup();
}

int General::startup() {
    phase = 1;
    mypixel.brightness(100);
    while(phase < 4){
        switch_pressed = myswitch.check_tact();
        toggle_stat = myswitch.check_toggle();
        if (phase < 3) {
            if (mode == 1) {
                mypixel.multis(0, 15, 255, 0, 0);
            } else if (mode == 2) {
                mypixel.multis(0, 15, 0, 0, 255);
            } else if (mode == 3) {
                mypixel.multis(0, 15, 255, 0, 255);
            }
            mypixel.unis(startPIXELs[startcord], 0, 255, 0);
            mypixel.closests(ball.get_azimuth(), 255, 255, 0);
        }
        switch (phase) {
            case 1:
                if (switch_pressed == 1){
                    mode = 1;
                    phase = 2;
                    mybuzzer.start(200, 4);
                    break;
                } else if (switch_pressed == 2) {
                    mode = 2;
                    phase = 2;
                    mybuzzer.start(200, 4);
                    break;
                } else if (switch_pressed == 3) {
                    mode = 3;
                    phase = 2;
                    mybuzzer.start(200, 4);
                    break;
                }
            
            case 2:
                if (switch_pressed == 1){
                    phase = 1;
                    mybuzzer.start(100, 4);
                    break;
                } else if (switch_pressed == 2) {
                    if (startcord < 4) {
                        startcord += 1;
                    } else {
                        startcord = 0;
                    }
                    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
                    mybuzzer.start(300, 4);
                    delay(100);
                    mybuzzer.start(300, 4);
                    break;
                } else if (switch_pressed == 3) {
                    phase = 3;
                    mybuzzer.start(200, 4);
                    break;
                }
            case 3:
                if (switch_pressed == 1){
                    phase = 2;
                    mybuzzer.start(100, 4);
                    break;
                } else if (switch_pressed == 2) {
                    gam.dir_reset();
                    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
                    mybuzzer.start(300, 1);
                    break;
                } else if (switch_pressed == 3) {
                    break;
                } else if (toggle_stat == 1) {
                    Run = true;
                    phase = 4;
                } else {
                    mybuzzer.start(400, 8);
                    delay(100);
                    mybuzzer.start(400, 8);
                    delay(500);
                }
        }
    }
    mypixel.brightness(999);
    return Run;
}

int General::check_run() {
    if (myswitch.check_toggle() == 0) {
        Run = false;
    } else {
        Run = true; 
    }
    return Run;
}

int General::check_mode() {
    return mode;
}

void General::update() {
    ball.read();
    if (usePIXEL) {
        mypixel.closest(ball.get_azimuth(), 255, 0, 0, 1);
    }
}
