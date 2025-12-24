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
        // if (ball.get_intensity() > 5) near = 1;
        // else near = 0;
        near = 0;

        if (abs(ball_azimuth) <= 15) {
            movement_azimuth = 0;
            speed = 210;
            turn = 1;
        } else if (15 < abs(ball_azimuth) && abs(ball_azimuth) <= 70) {
            movement_azimuth = ball_azimuth * 1.4;
            speed = 210;
            turn = 1;
        } else if (70 < abs(ball_azimuth) && abs(ball_azimuth) <= 100) {
            movement_azimuth = ball_azimuth * 1.5;
            speed = 210;

            turn = 0;
        } else if (100 < abs(ball_azimuth) && abs(ball_azimuth) <= 140) {
            movement_azimuth = ball_azimuth * 1.4;
            speed = 210;

            turn = 0;
            if (ball_azimuth >= 0) side = 0;
            else side = 1;
        } else {
            speed = 0;
        }
        // else if (80 < abs(ball_azimuth) && abs(ball_azimuth) <= 140) {
        //     if (near == 1) movement_azimuth = ball_azimuth * 1.5;
        //     else movement_azimuth = ball_azimuth * 1.4;
        //     speed = 210;
        // } else if (140 < abs(ball_azimuth)) {
        //     movement_azimuth = ball_azimuth * 1.3;
        //     speed = 210;
        // }

        movement_azimuth = (movement_azimuth + 360) % 360;

        if (turn == false) {
            mymotor.run(movement_azimuth, speed, 0);
        } else {
            if (side == 1) mymotor.run(movement_azimuth, speed, 25);
            else mymotor.run(movement_azimuth, speed, 335);
        }
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