#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::attack_() {
    if (line.get_type() != 0) {
        mymotor.run(line.get_eazimuth(), 200, 0);
    } else if (ball.get_stat() == 1) {
        float ball_azimuth = ball.get_azimuth();
        if (ball_azimuth > 180) ball_azimuth -= 360;
        ball_azimuth *= 1.6;
        if (ball_azimuth < 0) {
            ball_azimuth += 360;
        }
        mymotor.run(ball_azimuth, 200, 0);
    } else {
        mymotor.free();
    }
}