#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::attack_() {
    // mymotor.run(1, 200, 0);
    if (1 == false) {
        //pass
    } else if (ball.get_value(99) != 0) {
        ball.read();
        ball_dir = ball.get_azimuth();
        mypixel.closest(ball_dir, 255, 0, 0, 0);
        if (ball_dir < b_r1_deg || ball_dir > (360 - b_r1_deg)) {
            //if ((ball.get_magnitude() > ball_catch[0] && ball.get_magnitude() < ball_catch[1]) || catch_stat == 1) {
            //    catch_stat = 1;
            //    attack.b_p4();
            //}
            attack.b_p1();
        } else if (ball.get_magnitude() > brr) {
            catch_stat = 0;
            attack.b_p2();
        } else {
            catch_stat = 0;
            attack.b_p3();
        }
    } else {
        catch_stat = 0;
        mymotor.run(0, 0, 0);
    }
    // Serial.println(catch_stat);
    // Serial.println(ball.get_magnitude());
    mypixel.closest(mymotor.get_azimuth(), 255, 50, 255, 3);
}

void Attack::b_p1() {
    int ball_azimuth = ball.get_azimuth();
    if (ball.get_azimuth() > 180) {
        ball_azimuth = ball.get_azimuth() - 360;
    }
    movedir = ball_azimuth * appraoch_value;
    movedir = (movedir + 360) % 360;
    mymotor.run(movedir, b_r1speed, 0);
}

void Attack::b_p2() {
    movedir = ball.get_azimuth() + (degrees(asin(constrain(brr / ball.get_magnitude(), -1, 1))) * 3);
    movedir %= 360;
    mymotor.run(movedir, b_r2speed, 0);
}

void Attack::b_p3() {
    movedir = ball.get_azimuth() + (90 + (brr - ball.get_magnitude()) * 90 / brr);
    movedir %= 360;
    mymotor.run(movedir, b_r3speed, 0);
}

void Attack::b_p4() {
    myvector.get_plpocord(0, goalcord);
    int dir = myvector.get_azimuth(myvector.get_x(), myvector.get_y());
    mymotor.run(0, b_r4speed, dir);
}
