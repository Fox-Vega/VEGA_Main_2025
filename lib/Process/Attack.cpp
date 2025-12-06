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
        speed = 0;
        ball_azimuth = ball.get_azimuth();
        if (ball_azimuth > 180) ball_azimuth -= 360; //-179 ~ 180
        for (int i = 0; i < (wrap_size - 1); i++) {//-1が最適
            if (wrap[i][0] <= abs(ball_azimuth) && abs(ball_azimuth) <= wrap[i + 1][0]) {
                speed = wrap[i + 1][2];
                movement_azimuth = ball_azimuth * wrap[i + 1][1];
                if (wrap[i + 1][1] == 999) movement_azimuth = 0;
                break;
            }
        }
        movement_azimuth = (movement_azimuth + 360) % 360;
        mymotor.run(movement_azimuth, speed, 0);
    } else { //ラインない　ボールない
        if (!back) {
            back = true;
            back_start = millis();
        }

        //ボールがない時の後退
        if ((millis() - back_start) < back_border) mymotor.run(180, back_speed, 0);
        else mymotor.free();
    }
}