#include "Test.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Test::test_() {
    if (myswitch.check_tact() == 1) {
        t_mode -= 1;
        if (t_mode <= 0) {
            t_mode = 5;
        }
        delay(300);
    } else if (myswitch.check_tact() == 3) {
        t_mode += 1;
        if (t_mode >= 6) {
            t_mode = 1;
        }
        delay(300);
    }
    if (myswitch.check_toggle() == 1) {
        switch(t_mode) {
            case 1:
                mypixel.multi(0, 15, 255, 0, 0);
                mypixel.multi(4, 12, 255, 255, 255);
                mypixel.show();
                break;
            case 2:
                mypixel.multi(0, 15, 255, 0, 0);
                mypixel.show();
                break;
        }
        if ((millis() - lastbuzzer) > 500) {
            mybuzzer.start(400, 50);
            lastbuzzer = millis();
        }
    } else {
        switch(t_mode) {
            case 1:
                test.input();
                break;
            case 2:
                test.ir();
                break;
        }
    }
}


void Test::input() {
    ball.read();
    mypixel.multi(0, 15, 255, 255, 255);
    if (myswitch.check_tact() == 2){
        gam.dir_reset();
        mybuzzer.start(400, 50);
        delay(300);
    } else { //ゴールを向き続けるように
        int goal_azimuth = 0 - gam.get_azimuth();
        if (goal_azimuth < 0) {
            goal_azimuth += 360;
        }
        mypixel.closest(goal_azimuth, 255, 0, 100, 3); //7
    }
    if (ball.get_magnitude() != 0) {
        int value = constrain(ball.get_magnitude(), 0, 255); //入力値を0~255の範囲に制限
        int r, g, b;
        r = 255;
        g = 255 - value; //緑をよりゆるやかに減らす
        b = 0;
        mypixel.closest(ball.get_azimuth(), r, g, b, 1);
    }
    mypixel.show();
}

void Test::ir() {
    // Serial.print(">Ball_mag:");
    // Serial.println(ball.get_magnitude());
    // mypixel.multi(0, 15, 255, 255, 255);
    // mypixel.closest(ball.get_azimuth(), r, g, b, 1);
    // mypixel.show();
}

// void Test::goal() {
    
// }