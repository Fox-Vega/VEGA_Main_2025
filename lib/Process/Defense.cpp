// #include <Defense.h>
// #include <Input.h>
// #include <Output.h>
// #include <Process.h>
// #include <AIP.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <Arduino.h>
// #include <math.h>


// void DEFENCE::setup() {
//     //初期設定
//     Dline.deg = 0;
//     Dline.dist = 0;
//     Dline.detect = false;
//     Dball.deg = 0;
//     Dball.dist = 0;
//     Dball.flag = 999;
//     Dball.detect = false;
//     go_ang = 0; //初期値
// }

// void DEFENCE::defense_(void) {
//     Dline.deg = line.get_azimuth();
//     Dline.dist = line.get_dist();
//     Dline.detect = line.read();
//     if(Dline.dist>linedist)
//     {
//         //motor.run(azimuth, distance,0);
//     }
//     else
//     {
//         //ディフェンスする
//         int ballDeg = ball.get_azimuth();
//         int ballDist = ball.get_magnitude();
//         int ballflag1 =1; //ボールの位置　０左　１真ん中　２右 ３後ろ（詰み☆）
//         if(ballDeg < 90 || ballDeg > 270) {
//             ballflag1 = 0; //左
//         } else if(ballDeg >= 90 && ballDeg <= 270) {
//             ballflag1 = 1; //真ん中
//         } else if(ballDeg > 270 && ballDeg < 360) {
//             ballflag1 = 2; //右
//         } else {
//             ballflag1 = 3; //後ろ
//         }
//         if(ballflag1 == 0) {//左に動くとき
//             if(linedist<linecentor)//左に動くだけ
//             {
//                 motor.run(azimuth, distance, 0);
//             } else if(linedist>=linecentor) {
//                 //左に動く
//                 motor.run(azimuth, distance, -1);
//             }
//         } else if(ballflag1 == 1) {

//         } else if(ballflag1 == 2) {

//         } else {
//             ballflag1 = 3; //後ろ
//         }
//         if(ballflag1 == 0) {//左に動くとき
//             if(linedist<linecentor)//左に動くだけ
//             {
//                 motor.run(azimuth, distance, 0);
//             } else if(linedist>=linecentor) {
//                 //左に動く
//                 motor.run(azimuth, distance, -1);
//             }
//         } else if(ballflag1 == 1) {

// //         } else if(ballflag1 == 2) {

// //         } else {
// //             MyPIXEL.set_color(255, 0, 0); //後ろは赤色
// //             MyPIXEL.show();
// //         }
// // }
// // }