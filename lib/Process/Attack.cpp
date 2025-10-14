#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::setup() {
    wrap_size = sizeof(wrap) / sizeof(wrap[0]);
}

void Attack::attack_() {
    if (line.get_type() != 0) { //ラインある
        mymotor.run(line.get_eazimuth(), avoid_speed, 0);
    } else if (ball.get_stat() == 1) { //ラインない　ボールある
        mypixel.clear();
        back = false;
        speed = 0;
        ball_azimuth = ball.get_azimuth();
        if (ball_azimuth > 180) ball_azimuth -= 360; //-179 ~ 180
        for (int i = 0; i < (wrap_size - 1); i++) {//-1が最適
            if (wrap[i][0] <= abs(ball_azimuth) && abs(ball_azimuth) <= wrap[i + 1][0]) {
                speed = wrap[i + 1][2];
                movement_azimuth = ball_azimuth * wrap[i + 1][1];

                //TODO
                // if (i == 0) {
                //     mypixel.multi(0, 15, 255, 0, 0);
                // } else if (i == 1) {
                //     mypixel.multi(0, 15, 255, 255, 0);
                // } else if (i == 2) {
                //     mypixel.multi(0, 15, 0, 255, 0);
                // } else if (i == 3) {
                //     mypixel.multi(0, 15, 0, 0, 255);
                // }
            }
        }
        if (movement_azimuth < 0.0) movement_azimuth += 360.0;
        mymotor.run(movement_azimuth, speed, 0);

        //TODO
        // Serial.print(movement_azimuth);
        // Serial.print(" ");
        // mypixel.closest(movement_azimuth, 255, 255, 255, 1);
        // mypixel.closest(ball_azimuth, 255, 100, 100, 1);
    } else { //ラインない　ボールない
        mypixel.multi(0, 15, 255, 0, 0);
        if (!back) {
            back = true;
            back_start = millis();
        }

        //ボールがない時の後退
        if ((millis() - back_start) < back_border) mymotor.run(180, back_speed, 0);
        else mymotor.brake();
    }
}