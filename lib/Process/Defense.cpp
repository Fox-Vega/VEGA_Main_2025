#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

//todo
//ベクトルで主に動くように
//シリアルコマンドも実装

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
    d_timer.reset();
    silentTime.reset();

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

    //ボールのベクトルを算出
    myvector.get_cord(ball_azimuth,ball_dist);
    vector.ball_x = myvector.get_x();
    vector.ball_y = myvector.get_y();
    if(ball_azimuth<7||ball_azimuth>353){
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

    vector.line_x = line_x;
    vector.line_y = line_y;
    if (line_detect){
        if(abs(line_x)>5){//角か縦　type分類も追加したいな
            vector.line_ang=line_azimuth;
            vector.line_power=line_dist*40;
        } else {//これは直線ライン　角度は前後ろのみ
            vector.line_ang=line_y<0?180:0;
            vector.line_power=line_dist*20;
        }
        mybuzzer.stop();
    } else {
        mybuzzer.start(1000,999);
        vector.go_ang=lastdetect;
        vector.move_power=200;
        return r;
    }
    if(vector.ball_power<30){
        vector.move_power=vector.line_power;
        vector.go_ang=vector.line_ang;
    }
    else{
    if(false){
        vector.move_power=vector.line_power;
        vector.go_ang=vector.line_ang;
    } else {
        myvector.get_cord(vector.line_ang,vector.line_power);//ベクトル合成のためにxyを統一
        vector.move_y=myvector.get_y()*line_late;
        if(vector.move_y>line_max)vector.move_y=line_max;

        myvector.get_cord(vector.ball_ang,vector.ball_power);
        vector.move_x=myvector.get_x();

        vector.go_ang=myvector.get_azimuth(vector.move_x,vector.move_y);
        }
    }
    return r;
}

void Defense::move(){
    if(silentTime.read_milli()>7000){
        dash();
    } else {
        Serial.print("vector:");
        Serial.print("  ball: (");
        Serial.print(vector.ball_x);
        Serial.print(", ");
        Serial.print(vector.ball_y);
        Serial.print(") ");
        Serial.print("  line: (");
        Serial.print(vector.line_x);
        Serial.print(", ");
        Serial.print(vector.line_y);
        Serial.print(") ");
        Serial.print("  move: (");
        Serial.print(vector.move_x);
        Serial.print(", ");
        Serial.print(vector.move_y);
        Serial.print(",");
        Serial.print(vector.go_ang);
        Serial.print(", ");
        Serial.print(vector.move_power);
        Serial.println(") ");
        last_power=vector.move_power;
        if(vector.move_power<30){
            mymotor.free();
        } else {
            silentTime.reset();
            mymotor.run(vector.go_ang,vector.move_power,0);
        }
        mypixel.closest(vector.line_ang,0,255,0,1,1);
    }
}

void Defense::resetUI(){
    //背景 #39c5bbff
    background.red = 57;
    background.green = 197;
    background.blue = 187;
    background.alpha = 1;
    //ボール #ff00007f
    P_ball.red = 255;
    P_ball.green = 0;
    P_ball.blue = 0;
    P_ball.alpha = 0.5;
    //ライン #00b4787f
    P_line.red = 0;
    P_line.green = 180;
    P_line.blue = 120;
    P_line.alpha = 0.5;
    //前方の角度　#FF00647F
    P_r_azimuth.red = 255;
    P_r_azimuth.green = 0;
    P_r_azimuth.blue = 100;
    P_r_azimuth.alpha = 0.5;
    //進行方向
    go_ang.red = 0;
    go_ang.green = 255;
    go_ang.blue = 0;
    go_ang.alpha = 1;
}

void Defense::applyUI(){
    mypixel.multi(0, 15, background.red, background.green, background.blue,background.alpha);
    mypixel.closest(line_azimuth,P_line.red,P_line.green,P_line.blue,P_line.alpha,3);
    mypixel.closest(ball_azimuth,P_ball.red,P_ball.green,P_ball.blue,P_line.alpha,3);
    mypixel.closest(r_azimuth,P_r_azimuth.red,P_r_azimuth.green,P_r_azimuth.blue,P_r_azimuth.alpha,3);
    mypixel.closest(vector.go_ang,go_ang.red,go_ang.green,go_ang.blue,go_ang.alpha,3);
}

void Defense::ReadCommand(){2};

void Defense::dash(){
    silentTime.reset();
    d_timer.reset();
    while(d_timer.read_milli()<1000){
    get_value();
    mymotor.run(ball_azimuth,100,0);
    }
};

bool Defense::useSerial(bool use) {
    SerialEnabled = use;
    return SerialEnabled;
}