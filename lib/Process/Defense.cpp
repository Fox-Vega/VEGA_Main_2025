#include <Defense.h>
#include <Line.h>
#include <Input.h>
#include <Output.h>
#include <Process.h>
#include <AIP.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Arduino.h>
#include <math.h>


extern LINE line;

void Defense::setup(void) {
    //初期設定
    Dline.deg = 0;
    Dline.dist = 0;
    Dline.detect = false;
    Dball.deg = 0;
    Dball.dist = 0;
    Dball.flag = 999;
    Dball.detect = false;
}

void Defense::defense_(void) {
    Dline.deg = line.get_azimuth();
    Dline.dist = line.get_dist();
    Dline.detect = line.read();
    if(Dline.dist>linedist)
    {
        //motor.run(azimuth, distance,0);
    }
    else
    {
        //ディフェンスする
        Dball.deg = ball.get_azimuth();
        Dball.dist = ball.get_magnitude();
        int ballflag1 = 1; //ボールの位置　０左　１真ん中　２右 ３後ろ（詰み☆）
        if(Dball.deg < 90 || Dball.deg > 270) {
            ballflag1 = 0; //左
        } else if(Dball.deg >= 90 && Dball.deg <= 270) {
            ballflag1 = 1; //真ん中
        } else if(Dball.deg > 270 && Dball.deg < 360) {
            ballflag1 = 2; //右
        } else {
            ballflag1 = 3; //後ろ
        }
        Dball.flag = ballflag1;
    //     if(Dball.flag == 0) {//左に動くとき
    //         if(linedist<linecentor)//左に動くだけ
    //         {
    //             //motor.run(azimuth, distance, 0);
    //         } else if(linedist>=linecentor) {
    //             //左に動く
    //             //motor.run(azimuth, distance, -1);
    //         }
    //     } else if(Dball.flag == 1) {

    //     } else if(Dball.flag == 2) {

    //     } else {
    //         Dball.flag = 3; //後ろ
    //     }
    //     if(Dball.flag == 0) {//左に動くとき
    //         if(linedist<linecentor)//左に動くだけ
    //         {
    //             //motor.run(azimuth, distance, 0);
    //         } else if(linedist>=linecentor) {
    //             //左に動く
    //             //motor.run(azimuth, distance, -1);
    //         }
    //     } else if(Dball.flag == 1) {

    //     } else if(Dball.flag == 2) {

    //     } else {
    //         //MyPIXEL.set_color(255, 0, 0); //後ろは赤色
    //         //MyPIXEL.show();
    //     }
    // }
}
