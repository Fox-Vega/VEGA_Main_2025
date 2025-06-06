#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::attack_() {
    // mymotor.run(1, 200, 0);
    if (line.read() == false) {
        //pass
    } else if (ball.get_magnitude() != 0) {
        ball.read();
        ball_dir = ball.get_azimuth();
        mypixel.closest(ball_dir, 255, 0, 0, 0);
        if (ball_dir < b_r1_deg || ball_dir > 360 - b_r1_deg) {
            attack.b_p1();
        } else if (ball.get_magnitude() > brr) {
            attack.b_p2();
        } else {
            attack.b_p3();
        }
    } else {
        mymotor.run(0, 0, 0);
    }
}

void Attack::b_p1() {
    movedir = ball.get_azimuth() * appraoch_value;
    mymotor.run(movedir, b_r1speed, 0);
}

void Attack::b_p2() {
    movedir = ball.get_azimuth() + degrees(asin(brr / ball.get_magnitude()));
    mymotor.run(movedir % 360, b_r2speed, 0);
}

void Attack::b_p3() {
    movedir = ball.get_azimuth() + (90 + (brr - ball.get_magnitude()) * 90 / brr);
    mymotor.run(movedir % 360, b_r3speed, 0);
}