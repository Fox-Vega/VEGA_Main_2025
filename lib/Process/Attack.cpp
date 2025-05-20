// #include <Attack.h>

// void Attack::attack_() {
//     if (line.read() == true) {

//     } else {
//         ball.read();
//         ball_dir = ball.get_azimuth();
//         mypixel.closest(ball_dir, 255, 0, 0);
//         if (ball_dir < b_r1_deg || ball_dir > 360 - b_r1_deg) {
//             attack.b_p1();
//         } else if (ball.get_magnitude() > brr) {
//             attack.b_p2();
//         } else {
//             attack.b_p3();
//         }
//     }
// }

// void Attack::b_p1() {
//     face = 0;
//     if (ball.get_magnitude() < ballcatchvalue) {
//         movedir = ball.get_azimuth() * appraoch_value;
//         mymotor.run(movedir, b_r1speed, face);
//     } else {
//         myvector.get_plpocord(goalcords[0], goalcords[1]);
//         movedir = myvector.get_azimuth(myvector.get_plpo_x(), myvector.get_plpo_y()) - gyro.get_azimuth();
//         if (movedir < 0) {
//             movedir += 360;
//         }
//         face = myvector.get_azimuth(myvector.get_plpo_x(), myvector.get_plpo_y());
//         mymotor.run(movedir, goalspeed, face);
//     }
// }

// void Attack::b_p2() {
//     movedir = ball.get_azimuth() + asin(brr / ball.get_magnitude());
//     mymotor.run(movedir, b_r2speed, 0);
// }

// void Attack::b_p3() {
//     movedir = ball.get_azimuth() + 90 + (brr - ball.get_magnitude()) * 90 / brr;
//     mymotor.run(movedir, b_r3speed, 0);
// }