#include "General.h"
#include "Input.h"
#include "Output.h"


void General::setup() {
    Serial.begin(9600);
    mypixel.setup();
    mypixel.multi(0, 15, 255, 255, 255);
    mypixel.shows();
    mypixel.multi(0, 15, 255, 128, 0);
    mypixel.show();
    ball.setup();
    // line.setup();
    mymotor.setup();
    mybuzzer.setup();
    myswitch.setup();
    gam.setup();
    mypixel.multi(0, 15, 255, 255, 255);
    mypixel.shows();
    mybuzzer.preset(1);
}

void General::startup() {
    phase = 1;
    mypixel.brightness(150);
    while (phase < 4) {
        switch_pressed = myswitch.check_tact();
        toggle_stat = myswitch.check_toggle();
        if (phase < 3) {
            mypixel.clear();
            if (mode == 1) {
                mypixel.multi(0, 15, 255, 130, 130);
            } else if (mode == 2) {
                mypixel.multi(0, 15, 130, 255, 130);
            } else if (mode == 3) {
                mypixel.multi(0, 15, 255, 255, 90);
            }
            int d = 0 - gam.get_azimuth();
            if (d < 0) {
                d += 360;
            }
            mypixel.closest(d, 255, 50, 50, 3);
            mypixel.closest(d, 255, 0, 100, 1);
            if (startPIXELs[startcord] == 99) {
                mypixel.uni(2, 255, 255, 255);
                mypixel.uni(6, 255, 255, 255);
                mypixel.uni(10, 255, 255, 255);
                mypixel.uni(14, 255, 255, 255);
            } else {
                mypixel.uni(startPIXELs[startcord], 255, 255, 255);
            }
            ball.read();
            if (ball.get_magnitude() != 0) {
                mypixel.closest(ball.get_azimuth(), 255, 50, 255, 1);
            }
            mypixel.shows();
        }
        switch (phase) {
            case 1:
                if (switch_pressed == 1){
                    mode = 1;
                    phase = 2;
                    mybuzzer.start(500, 200);
                } else if (switch_pressed == 2) {
                    mode = 2;
                    phase = 2;
                    mybuzzer.start(500, 200);
                } else if (switch_pressed == 3) {
                    mode = 3;
                    phase = 2;
                    mybuzzer.start(500, 200);
                }
                delay(100);
                break;
            case 2:
                if (switch_pressed == 1){
                    phase = 1;
                    startcord = 0;
                    mode = 0;
                    mybuzzer.start(100, 500);
                } else if (switch_pressed == 2) {
                    if (startcord < 4) {
                        startcord += 1;
                    } else {
                        startcord = 0;
                    }
                    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
                    mybuzzer.start(500, 50);
                    mybuzzer.start(500, 50);
                } else if (switch_pressed == 3) {
                    phase = 3;
                    mybuzzer.start(500, 500);
                    mypixel.clear();
                    mypixel.shows();
                }
                break;
            case 3:
                if (switch_pressed == 1){
                    phase = 2;
                    mybuzzer.start(100, 500);
                } else if (switch_pressed == 2) {
                    gam.dir_reset();
                    mybuzzer.start(300, 500);
                } else if (switch_pressed == 3) {
                    //機能無し
                } else if (toggle_stat == 1) {
                    Run = true;
                    phase = 4;
                } else {
                    mypixel.rainbow();
                    if (millis() - lastbuzzer > 500) {
                        mybuzzer.start(400, 50);
                        lastbuzzer = millis();
                    }
                }
                break;
        }
    }
    mypixel.brightness(999);
    mypixel.clear();
    mypixel.shows();
    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
}

int General::get_run() {
    if (myswitch.check_toggle() == 0) {
        Run = false;
    } else {
        Run = true;
    }
    return Run;
}

int General::get_mode() {
    return mode;
}