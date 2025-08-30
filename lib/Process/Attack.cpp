#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::attack_() {
    // mymotor.run(1, 200, 0);
    line.read();
    if (line.get_magnitude() != 999) {
        mymotor.run(line.get_avoid(), avoid_speed, 0);
        mybuzzer.start(330, 999);
        mypixel.closest(line.get_avoid(), 50, 255, 50,1, 3);

    } else if (ball.get_value(99) != 0) {
        mybuzzer.stop();
        ball.read();
        mypixel.closest(ball_dir, 255, 0, 0,1, 1);
        ball_dir = ball.get_azimuth();

        if (abs((ball_dir + 180) % 360 - 180) < b_r1_deg) {
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
        mybuzzer.stop();
        catch_stat = 0;
        mymotor.free();
    }
}

void Attack::b_p1() {
    movedir = ((ball_dir + 180) % 360 - 180) * appraoch_value + 1080;
    movedir %= 360;
    // mymotor.run(movedir, b_r1speed, 0);
    mymotor.run(0, b_r1speed, 0);
}

void Attack::b_p2() {
    if (ball.get_azimuth() < 180) {
        movedir = ball.get_azimuth() + p2;
    } else {
        movedir = ball.get_azimuth() - p2 + 360;
    }
    movedir %= 360;
    mymotor.run(movedir, b_r2speed, 0);
}

void Attack::b_p3() {
    if (ball.get_azimuth() < 180) {
        movedir = ball.get_azimuth() + 90;
    } else {
        movedir = ball.get_azimuth() - 90;
    }
    movedir %= 360;
    mymotor.run(movedir, b_r3speed, 0);
}

// void Attack::b_p4() {
// }
