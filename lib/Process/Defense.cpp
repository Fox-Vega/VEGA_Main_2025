#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

//todo
//ベクトルで主に動くように
//ピクセルのUIも整えたい
//シリアルコマンドも実装
//ベクトル合成でｐとかでできるだけ分けないようにしたい

//powershell -ExecutionPolicy Bypass -File C:\Users\kusas\Desktop\program\auto-commit.ps1



void Defense::setup(void){
    // vectorの全要素を999で初期化
    vector.ball_ang = 999;
    vector.ball_power = 999;
    vector.ball_x = 999;
    vector.ball_y = 999;
    vector.line_ang = 999;
    vector.line_power = 999;
    vector.line_x = 999;
    vector.line_y = 999;
    vector.go_ang = 999;
    vector.move_power = 999;
    vector.move_x = 999;
    vector.move_y = 999;
}

void Defense::defense_(void){
    resetUI();
    d_timer.reset();
    if(SerialEnabled){
        ReadCommand();
    }
    get_value();
    cal_vector();
    move();
    applyUI();
}

void Defense::get_value(){
    line.read();
    ball.read();

    r_azimuth = gam.get_azimuth();
    line_detect=line.get_magnitude()==999 ? false : true;
    if(line_detect){
        line_azimuth=line.get_azimuth();
        line_absolute_angle=norm360(line_azimuth - r_azimuth);
        line_dist=line.get_magnitude()/10;
        lastdetect=line_azimuth;
        myvector.get_cord(line_azimuth,line_dist);
        line_x=myvector.get_x();
        line_y=myvector.get_y();
    } else {;
        line_azimuth=999;
        line_absolute_angle=999;
        line_dist=999;
        line_x=999;
        line_y=999;
    }

    ball_azimuth=ball.get_azimuth();
    ball_absolute_angle=norm360(ball_azimuth - r_azimuth);
    ball_dist=ball.get_magnitude();
}

int Defense::cal_vector(){
    int r=999;
    myvector.get_cord(ball_azimuth,ball_dist);
    vector.ball_x = myvector.get_x();
    vector.ball_y = myvector.get_y();
    if(ball_azimuth<15||ball_azimuth>345){
        vector.ball_ang=999;
        vector.ball_power=0;
    } else {
        if(vector.ball_x<0){
            vector.ball_ang=270;
        } else {
            vector.ball_ang=90;
        }
        vector.ball_power=200;
    }

    if (line_detect){
        if(abs(line_x)>5){
            vector.line_ang=line_azimuth;
            vector.line_power=line_dist*20;
        } else {
            vector.line_ang=
        }
    } else {
        vector.go_ang=lastdetect;
        vector.move_power=200;
        return r;
    }
    return r;
}
void Defense::move(){
    mymotor.run(vector.go_ang,vector.move_power,0);
}

void Defense::resetUI(){
    //背景
    background.red = 57;
    background.green = 197;
    background.blue = 187;
    background.alpha = 100;
}

void Defense::applyUI(){

    background.red = background.red*(background.alpha/100);
    background.green = background.green*(background.alpha/100);
    background.blue = background.blue*(background.alpha/100);

    mypixel.multi(0, 15, background.red, background.green, background.blue);
    mypixel.closest(line_azimuth,255,0,0,1);
    Serial.println(line_x);
    Serial.println(line_y);
}

void Defense::ReadCommand(){}

bool Defense::useSerial(bool use) {
    SerialEnabled = use;
    return SerialEnabled;
}