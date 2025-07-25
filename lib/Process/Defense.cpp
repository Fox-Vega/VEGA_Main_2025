#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

#define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })//Serial.printのprintfフォーマット版

void Defense::setup(void){mypixel.use_pixel(true);}

void Defense::defense_(void){
        gotVector=0;
        mypixel.multi(0, 15, 0, 0, 0);
        if(myswitch.check_toggle()==1){
        get_vector();
        if(line_detect){
            // if(line_type==1)p1();
            // else if(line_type==2)p2();
            trace();
        }
        else{
            GoBackLine();
        }
    }
    else{
        mymotor.free();
        mypixel.rainbow();
        get_vector();
    }
}



void Defense::get_vector(void){
    // ライン情報の取得
    line.read();
    line_azimuth = line.get_azimuth();
    line_dist = line.get_magnitude();
    line_type = line.get_type();
    line_detect = (line_dist == 999) ? false : true;
    myvector.get_cord(line_azimuth, line_dist);
    line_x = myvector.get_x();
    line_y = myvector.get_y();
    if(line_detect){
        if(line_azimuth<45&&line_azimuth>315) lastdetect= 0;
        else if(line_azimuth<225&&line_azimuth>135) lastdetect= 180;
        else lastdetect = line_azimuth;
    }
    //計算
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

    // ボール情報の取得
    ball.read();
    ball_azimuth = ball.get_azimuth();
    ball_dist = (int)ball.get_magnitude();
    ball_detect = ((int)ball.get_magnitude() > 0) ? true : false;
    myvector.get_cord(ball_azimuth, ball_dist);
    ball_x = myvector.get_x();
    ball_y = myvector.get_y();
    //計算
    if(ball_detect){
        myvector.get_cord(ball_azimuth, ball_dist);
        int ball_x = myvector.get_x();
        int ball_y = myvector.get_y();
        printf_s(">ball_x:%d\n>ball_y:%d\n", ball_x, ball_y);
        // ここで処理
        ball_x= ball_x> ball_max_X ? ball_max_X : ball_x;
        ball_power = (ball_x/30)*15+20*(ball_x>0 ? 1 : -1); // ボールの位置に応じてパワーを調整
        ball_power = ball_x<catch_ball_X ? 0 : ball_power; // 中央にいるときはボールを蹴らない
        // -だったら左に（270）、+だったら右に
        ball_go_ang = (ball_x < 0) ? 270 : 90;
    }
    else{
        ball_power = 0;
        ball_go_ang = 0;+
    }

    //ジャイロ
    r_azimuth = gam.get_azimuth();

    gotVector=1;// ベクトルを取得したフラグを立てる
}


void Defense::p1(void){
    move_x=ball_power;
    move_y=line_power*(line_go_ang==0 ? 1 : -1);
    move_power = myvector.get_magnitude(move_x, move_y*line_rate);
    int move_azimuth = myvector.get_azimuth(move_x, move_y*line_rate);
    if(ball_power==0)trace();
    else if(line_dist<2) {mymotor.run(ball_go_ang,ball_power,0);mypixel.closest(ball_go_ang, 255, 0, 0, 3);}
    else {mymotor.run(move_azimuth, move_power,0);mypixel.closest(move_azimuth, 0, 255, 0, 3);}
}

void Defense::p2(void){
    int line_azimuth_mod[2] = {999,999};
    if(line_azimuth>0&&line_azimuth<90)
    else if(line_azimuth>90&&line_azimuth<180)
    else if(line_azimuth>180&&line_azimuth<270)
    else if(line_azimuth>270&&line_azimuth<360)
    else line_azimuth_mod = 999;
    if(line_azimuth_mod==999)p4();
    else{

    }
}

void Defense::trace(void){
    if(gotVector==0) get_vector();
    if(line_detect){
            if(line_dist>2){
                mymotor.run(line_go_ang, line_power*line_rate, 0);
            }
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
        p4();
    }
}

void Defense::p4(void){
    mypixel.closest(lastdetect,255,0,0,3);
    int backPower =(lastdetect<15&&lastdetect>345)? 200 : 80;
    mymotor.run(lastdetect,backPower,0);
    printf_s("noline:%d\n", lastdetect);
}

void Defense::debug1(void){
    printf_s("\n>debug1-line_azimuth:%d\n", line_azimuth);
    printf_s(">debug1-R_azimuth:%d\n", r_azimuth);
    printf_s(">debug1-line_type:%d\n", line_type);
}

