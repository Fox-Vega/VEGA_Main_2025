#include "Test.h"

void Test::gyro() {
    if (myswitch.check_tact() == 2){
        gam.dir_reset();
    } else { //ゴールを向き続けるように
        int goal_azimuth = 0 - gam.get_azimuth();
        if (goal_azimuth < 0) {
            goal_azimuth += 360;
        }
        if (myswitch.check_toggle() == 0) {
            mypixel.multi(0, 15, 255, 255, 0);
            mypixel.closest(goal_azimuth, 0, 0, 255, 8);
        } else {
            mypixel.multi(0, 15, 0, 0, 255);
            mypixel.closest(goal_azimuth, 255, 255, 0, 8);
        }
        mypixel.show();
    }
}

void Test::ir() {
    int value = constrain(ball.get_magnitude() / 2000.0f, 0, 255); //入力値を0~255の範囲に制限
    int r, g, b;
    if (value < 128) { 
        //0～127: 青 → 黄色（青減少 + 赤増加）
        b = 255 - (value * 2);
        r = value * 2;
        g = (value * 1.5); //緑を赤の値より少し控えめにすることで自然な黄色に
    } else { 
        //128～255: 黄色 → 赤（緑を段階的に減らす）
        r = 255;
        g = 255 - ((value - 128) * 1.5); //緑をよりゆるやかに減らす
        b = 0;
    }
    mypixel.closest(ball.get_azimuth(), r, g, b, 1);
}

// void Test::goal() {
    
// }