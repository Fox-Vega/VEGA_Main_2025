#include "General.h"
#include "Input.h"
#include "Output.h"


void General::setup() {
    ball.setup();
    gam.setup();
    // line.setup();
    mymotor.setup();
    mypixel.setup();
    mybuzzer.setup();
    myswitch.setup();
    Serial.begin(9600);
}

int General::startup() {
    phase = 1;
    while(phase != 4){
        switch_pressed = myswitch.check_tact();
        toggle_stat = myswitch.check_toggle();
        mypixel.brightness(100);
        switch (phase) {
            case 1:
                if (switch_pressed == 1){
                    mode = 1;
                    phase = 2;
                    mypixel.clear();
                    mypixel.multis(1, 16, 255, 0, 0);
                    mybuzzer.start(200, 4);
                    break;
                } else if (switch_pressed == 2) {
                    mode = 2;
                    phase = 2;
                    mypixel.clear();
                    mypixel.multis(1, 16, 0, 0, 255);
                    mybuzzer.start(200, 4);
                    break;
                } else if (switch_pressed == 3) {
                    mode = 3;
                    phase = 2;
                    mypixel.clear();
                    mypixel.multis(1, 16, 255, 0, 255);
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
                    }
                    mypixel.clear();
                    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
                    mypixel.unis(startPIXELs[startcord], 255, 0, 0);
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
                    mybuzzer.start(200, 2);
                    delay(100);
                    mybuzzer.start(100, 4);
                    break;
                } else if (switch_pressed == 3) {
                    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
                    mybuzzer.start(300, 4);
                    delay(100);
                    mybuzzer.start(300, 4);
                    break;
                } else if (toggle_stat == 1) {
                    Run = true;
                } else {
                    mybuzzer.start(300, 8);
                    delay(100);
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
    
}
