#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

#define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })

void Defense::setup(void){
    mybuzzer.start(1000, 200);
    general.setup();
}

void Defense::defense_(void){
    setup();
    while(true){
        if(myswitch.check_toggle()==1){
        get_vector();
        if(line_detect){
            if(line_dist>2)line_();
            else{mymotor.free(); printf_s("on line dist:%d\n", line_dist);}
            // mymotor.run(0, 0, 0);
            // mypixel.closest(line_azimuth, 0, 255, 0, 1);
            // printf_s("onLine dis:%d\n", line_dist);
        }
        else{
            GoBackLine();
        };
        delay(10);
    }
    else mymotor.free();
    }
}


void Defense::p1(void){
    // int ball_flag[2] = 0;//0:前 1:右 2:左
    // if(ball_azimuth>=330||ball_azimuth<=30){
    //     ball_flag=0;
    // }
    // else if(ball_azimuth<=180){
    //     ball_flag=1;
    // }
    // else{
    //     ball_flag=2;
    // };
    // int move_y = line_dist;

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
    int power = 130/(12-line_dist);
    go_ang = line_fb==2 ? 0 : 180;
    //mymotor.run(go_ang, power, 0);
    printf_s("line_fb:%s azimuth:%d\n", line_fb==1 ? "back" : "front", line_azimuth);
}
void Defense::GoBackLine(){
    mypixel.closest(lastdetect,255,0,0,1);
    mymotor.run(lastdetect,50,0);
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
}
