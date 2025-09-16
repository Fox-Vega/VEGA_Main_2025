#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::setup() {
    wrap_size = sizeof(wrap) / sizeof(wrap[0]);
}

void Attack::attack_() {
    if (line.get_type() != 0) {
        mymotor.run(line.get_eazimuth(), avoid_speed, 0);
    } else if (ball.get_stat() == 1) {
        int ball_azimuth = ball.get_azimuth();
        if (ball_azimuth > 180) ball_azimuth -= 360; //-179 ~ 180
        for (int i = 0; i < (wrap_size - 1); i++) {//-1が最適
            if (wrap[i][0] <= ball_azimuth && ball_azimuth <= wrap[i + 1][0]) movement_azimuth = ball_azimuth * (wrap[i + 1][1] / wrap[i + 1][0]);
        }
        if (movement_azimuth < 0) movement_azimuth += 360;
        mymotor.run(movement_azimuth, wrap_speed, 0);
    } else {
        mymotor.run(180, back_speed, 0);
    }
}