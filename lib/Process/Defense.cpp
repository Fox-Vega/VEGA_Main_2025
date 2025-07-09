#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

#define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })

void Defense::setup(void){
    mybuzzer.start(1000, 200);
    general.setup();
    mypixel.use_pixel(true);
}

void Defense::defense_(void){
    setup();
    while(true){
        if(myswitch.check_toggle()==1){
        get_vector();
        ball_();
    //     if(line_detect){
    //         p1();
    //         // if(line_dist>1)line_();
    //         // else{
    //         //     if(true!=(r_azimuth<5&&r_azimuth>355)){ //ジャイロの初期化
    //         //         mymotor.run(0,0,0);
    //         //         mypixel.multi(0,15,255,0,0);
    //         //         printf_s("reset dir\n");
    //         //     }
    //         //     else{
    //         //         mymotor.free();
    //         //         mypixel.clear();
    //         //     }
    //         //     printf_s("on line dist:%d\n", line_dist);
    //         // }
    //     }
    //     else{
    //         GoBackLine();
    //     };
    // }
    // else mymotor.free();
    // mypixel.show();
         }}
}


void Defense::p1(void){
    line_();
    move_x=ball_power;
    move_y=line_power*(line_go_ang==0 ? 1 : -1);
    move_power = myvector.get_magnitude(move_x, move_y);
    int move_azimuth = myvector.get_azimuth(move_x, move_y);
    mymotor.run(move_azimuth, move_power,0);
    mypixel.closest(move_azimuth, 0, 255, 0, 3);
}

// void Defense::p2(void){.
//     int type =999;
//     if(line_azimuth<=90)
//     else if(line_azimuth<=180)
//     else if()
//     else if()
// }

void Defense::line_(void){
    int line_fb = 0;
    if(line_azimuth<=90||line_azimuth>=270){//b
        line_fb = 1;
    }
    if(line_azimuth>90&&line_azimuth<270){//f
        line_fb = 2;
    }
    line_power = 125/(12-line_dist);
    if(line_power<50) line_power = 50;
    line_go_ang = line_fb==2 ? 180 : 0;
    if(line_dist<2) line_power = 0;
    // printf_s("line_fb:%s azimuth:%d\n", line_fb==2 ? "back" : "front", line_azimuth);
    // mypixel.closest(line_go_ang, 0, 255, 0, 3);
}

void Defense::ball_(void){
    if(ball_detect){
        myvector.get_cord(ball_azimuth, ball_dist);
        int ball_x = myvector.get_x();
        int ball_y = myvector.get_y();
        printf_s(">ball_x:%d\n>ball_y:%d\n", ball_x, ball_y);
        // ball_xの分類（-6 ～ 6）
        int category = 0;
        if(ball_x < 0) {
            if(ball_x < -500) category = -6;
            else if(ball_x < -400) category = -5;
            else if(ball_x < -300) category = -4;
            else if(ball_x < -200) category = -3;
            else if(ball_x < -100) category = -2;
            else if(ball_x < -50)  category = -1;
            else category = 0;
        } else {
            if(ball_x < 50) category = 0;
            else if(ball_x < 100) category = 1;
            else if(ball_x < 200) category = 2;
            else if(ball_x < 300) category = 3;
            else if(ball_x < 400) category = 4;
            else if(ball_x < 500) category = 5;
            else category = 6;
        }
        printf_s(">ball_x-category:%d\n", category);
    }
    else{
        mymotor.free();
        mypixel.clear();
        mybuzzer.start(1000, 200);
        printf_s("no ball\n");
    }
}

void Defense::GoBackLine(){
    mypixel.closest(lastdetect,255,0,0,3);
    mymotor.run(lastdetect,80,0);
    printf_s("noline:%d\n", lastdetect);
}

void Defense::get_vector(void)
{
    // ライン情報の取得
    line.read();
    line_azimuth = line.get_azimuth();
    line_dist = line.get_magnitude();
    line_detect = (line_dist == 999) ? false : true;
    line_x = line_dist * cos(radians(line_azimuth));
    line_y = line_dist * sin(radians(line_azimuth));
    if(line_detect){
        if(line_azimuth<45&&line_azimuth>315) lastdetect= 0;
        else if(line_azimuth<225&&line_azimuth>135) lastdetect= 180;
        else lastdetect = line_azimuth;
    }
    // ボール情報の取得
    ball.read();
    ball_azimuth = ball.get_azimuth();
    ball_dist = (int)ball.get_magnitude();
    ball_detect = ((int)ball.get_magnitude() > 0) ? true : false;
    myvector.get_cord(ball_azimuth, ball_dist);
    ball_x = myvector.get_x();
    ball_y = myvector.get_y();
    printf_s(">line_1:%d\n",line.get_value(0));
    //ジャイロ
    r_azimuth = gam.get_azimuth();
    printf_s(">r_azimuth:%d\n", r_azimuth);
}
