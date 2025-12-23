#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::setup() {
    wrap_size = sizeof(wrap) / sizeof(wrap[0]);
}

void Attack::attack_() {
    // mypixel.rainbow();
    if (line.get_type() != 0) { //ラインある
        mymotor.run(line.get_eazimuth(), avoid_speed, 0);
    } else if (ball.get_stat() == 1) { //ラインない　ボールある
        // mymotor.move(0);
        back = false;
        ball_azimuth = ball.get_azimuth();
        if (ball_azimuth > 180) ball_azimuth -= 360; //-179 ~ 180
        if (ball.get_magnitude() > 10) near = 0;
        else near = 1;

        if (abs(ball_azimuth) <= 15) {
            movement_azimuth = 0;
            speed = 210;
        } else if (15 < abs(ball_azimuth) && abs(ball_azimuth) <= 70) {
            movement_azimuth = ball_azimuth * 1.3;
            speed = 210;
        } else if (80 < abs(ball_azimuth) && abs(ball_azimuth) <= 140) {
            if (near == 1) movement_azimuth = ball_azimuth * 1.5;
            else movement_azimuth = ball_azimuth * 1.4;
            speed = 210;
        } else if (140 < abs(ball_azimuth)) {
            movement_azimuth = ball_azimuth * 1.3;
            speed = 210;
        }

        movement_azimuth = (movement_azimuth + 360) % 360;
        mymotor.run(movement_azimuth, speed, 0);
    } else { //ラインない　ボールない
        // if (!back) {
        //     back = true;
        //     back_start = millis();
        // }

        // //ボールがない時の後退
        // if ((millis() - back_start) < back_border) mymotor.run(180, back_speed, 0);
        // else mymotor.free();

        mymotor.free();
    }
}