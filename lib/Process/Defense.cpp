#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

#define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })

void Defense::setup(void){
    mybuzzer.start(1000, 200);
    mypixel.use_pixel(false);
}

void Defense::defense_(void){
    setup();
    while(true) {
        gotVector=0;
        mypixel.multi(0, 15, 0, 0, 0);
        if(myswitch.check_toggle()==1){
        get_vector();
        if(line_detect){
            p1();
        }
        else{
            GoBackLine();
        }
    }
    else{
        mymotor.free();
        mypixel.rainbow();
    }
mypixel.show();
}}

void Defense::trace(void){
    if(gotVector==0){
    get_vector();
    }
    if(line_detect){
        if(line_dist>2)mymotor.run(line_go_ang, line_power, 0);
        else if(true!=(r_azimuth<15&&r_azimuth>345)){
            mymotor.run(0,0,0);
            int d=  0-r_azimuth;
            if(d<0) d+=360;
            mypixel.closest(d, 255, 255, 255, 3);
        }
        else {
            mymotor.free();
            mypixel.multi(0, 15, 0, 0, 255);
        }
    }
    else{
        GoBackLine();
    }
}

void Defense::p1(void){
    line_();
    ball_();
    move_x=ball_power;
    move_y=line_power*(line_go_ang==0 ? 1 : -1);
    move_power = myvector.get_magnitude(move_x, move_y*0.75);
    int move_azimuth = myvector.get_azimuth(move_x, move_y);
    if(ball_power==0)trace();
    else if(line_dist<2) {mymotor.run(ball_go_ang,ball_power,0);mypixel.closest(ball_go_ang, 255, 0, 0, 3);}
    else {mymotor.run(move_azimuth, move_power,0);mypixel.closest(move_azimuth, 0, 255, 0, 3);}
}

void Defense::line_(void){
    int line_fb = 0;
    if(line_azimuth<=90||line_azimuth>=270){//b
        line_fb = 1;
    }
    if(line_azimuth>90&&line_azimuth<270){//f
        line_fb = 2;
    }
    line_power = 120/(13-line_dist);
    if(line_power<60) line_power = 60;
    line_go_ang = line_fb==2 ? 180 : 0;
    if(line_dist<2) line_power = 0;
    else mypixel.closest(line_azimuth, 0, 255, 0, 3);
}

void Defense::ball_(void){
    if(ball_detect){
        myvector.get_cord(ball_azimuth, ball_dist);
        int ball_x = myvector.get_x();
        int ball_y = myvector.get_y();
        printf_s(">ball_x:%d\n>ball_y:%d\n", ball_x, ball_y);
        // ball_xの分類（-6 ～ 6）
        category = 0;
        if(ball_x < 0) {
            if(ball_x < category_width*-6) category = -6;
            else if(ball_x < category_width*-5) category = -5;
            else if(ball_x < category_width*-4) category = -4;
            else if(ball_x < category_width*-3) category = -3;
            else if(ball_x < category_width*-2) category = -2;
            else if(ball_x < category_width*-1)  category = -1;
            else category = 0;
        } else {
            if(ball_x <  category_width) category = 0;
            else if(ball_x < category_width*2) category = 1;
            else if(ball_x < category_width*3) category = 2;
            else if(ball_x < category_width*4) category = 3;
            else if(ball_x < category_width*5) category = 4;
            else if(ball_x < category_width*6) category = 5;
            else category = 6;
        }
        printf_s(">ball_x-category:%d\n", category);

        ball_power = category *20 + 25*(category < 0 ? -1 : 1);
        if(category==0) ball_power = 0; // 中央にいるときはボールを蹴らない
        // -だったら左に（270）、+だったら右に
        ball_go_ang = (category < 0) ? 270 : 90;
    }
    else{
        category = 0;
        ball_power = 0;
        ball_go_ang = 0;
    }
}

void Defense::GoBackLine(){
    mypixel.closest(lastdetect,255,0,0,3);
    int backPower =(lastdetect<15&&lastdetect>345)? 200 : 80;
    mymotor.run(lastdetect,backPower,0);
    printf_s("noline:%d\n", lastdetect);
}

void Defense::get_vector(void){
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
    line_();
    ball_();
    gotVector=1;
}
