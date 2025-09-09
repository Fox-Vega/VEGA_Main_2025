#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"

//todo
//ベクトルで主に動くように
//シリアルコマンドも実装

//powershell -ExecutionPolicy Bypass -File C:\Users\kusas\Desktop\program\auto-commit.ps1

void Defense::timerReset(){
    d_timer.reset();
    silentTime.reset();
}

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
}

void Defense::defense_(void){
    resetUI();
    d_timer.reset();
    get_value();
    // Serial.println("get value:" + String(d_timer.read_milli()));
    d_timer.reset();
    cal_vector();
    // Serial.println("cal vector:" + String(d_timer.read_milli()));
    d_timer.reset();
    move();
    // Serial.println("move:" + String(d_timer.read_milli()));
    d_timer.reset();
    applyUI();
    // Serial.println("apply UI:" + String(d_timer.read_milli()));
    // Serial.println("--------------------------------");
    // Serial.println();
    // for (int i= 0; i<24; i++){
    //     Serial.print(line.get_value(i));
    //     if(i<23)Serial.print(",");
    // }
    // Serial.println();
}

void Defense::get_value(){
    r_azimuth = gam.get_azimuth();
    line_detect=line.get_magnitude()==999 ? false : true;
    if(line_detect){
        line_azimuth=norm360(line.get_azimuth());
        line_absolute_angle=norm360(line_azimuth - r_azimuth);
        line_dist=(line.get_magnitude()/100)+1;
        lastdetect=line_azimuth;
        myvector.get_cord(line_azimuth,line_dist);
        line_x=myvector.get_x();
        line_y=myvector.get_y();
    } else {
        line_azimuth=999;
        line_absolute_angle=999;
        line_dist=999;
        line_x=999;
        line_y=999;
    }

    ball_azimuth=ball.get_azimuth();
    ball_absolute_angle=norm360(ball_azimuth - r_azimuth);
}

int Defense::cal_vector(){
    int r=999;
    //ボールのベクトルを算出
    vector.ball_x = ball.get_x();
    vector.ball_y = ball.get_y();
    if(ball_azimuth<15||ball_azimuth>345){
        vector.ball_ang=999;
        vector.ball_power=0;
        Serial.println("on ball");
    } else {
        if(vector.ball_x<0){
            vector.ball_ang=270;
        } else {
            vector.ball_ang=90;
        }
        vector.ball_power=150;
        Serial.println("off ball");
    }

    vector.line_x = line_x;
    vector.line_y = line_y;
    if (line_detect){
        // Serial.println("line detected");
        if(abs(line_x)>5){//角か縦　type分類も追加したいな
            // Serial.println("corner");
            vector.line_ang=line_azimuth;
            vector.line_power=line_dist*40;
        } else {//これは直線ライン　角度は前後ろのみ
            // Serial.println("normal");
            vector.line_ang=line_y<0?180:0;
            vector.line_power=line_dist*20;
        }
        mybuzzer.stop();
    } else {
        mybuzzer.start(1000,999);
        vector.go_ang=-lastdetect;
        vector.move_power=200;
        return r;
    }

    if(vector.ball_power<30||ball.get_stat()==0){
        vector.move_power=vector.line_power;
        vector.go_ang=vector.line_ang;
        // Serial.println("on ball only line");
    } else if(diff_signs(ball_x,line_x)&&abs(line_x)>5){
        vector.move_power=vector.line_power;
        vector.go_ang=vector.line_ang;
    } else {
        // Serial.println("normal vec");
        myvector.get_cord(vector.line_ang,vector.line_power);//ベクトル合成のためにxyを統一

        vector.move_y=(myvector.get_y()*line_late);
        if(vector.move_y>line_max)vector.move_y=line_max;

        myvector.get_cord(vector.ball_ang,vector.ball_power);
        vector.move_x=myvector.get_x();

        // vector.go_ang=myvector.get_azimuth(vector.move_x,vector.move_y);
        // Serial.print("go ang:");
        Serial.println(vector.go_ang);
        double angle_rad = atan2(vector.move_y, vector.move_x); // y, x の順
        double angle_deg = angle_rad * 180.0 / M_PI;
        angle_deg = norm360(angle_deg + 90);
        vector.go_ang = angle_deg;
        vector.move_power=myvector.get_magnitude(vector.move_x,vector.move_y);
    }
    return r;
}

void Defense::move(){
    if(silentTime.read_milli()>7000){
        dash();
    } else {
        // Serial.print("vector:");
        // Serial.print("  ball: (");
        // Serial.print(vector.ball_ang);
        // Serial.print(", ");
        // Serial.print(vector.ball_power);
        // Serial.print(", ");
        // Serial.print(vector.ball_x);
        // Serial.print(", ");
        // Serial.print(vector.ball_y);
        // Serial.print(") ");
        // Serial.print("  line: (");
        // Serial.print(vector.line_ang);
        // Serial.print(", ");
        // Serial.print(vector.line_power);
        // Serial.print(", ");
        // Serial.print(vector.line_x);
        // Serial.print(", ");
        // Serial.print(vector.line_y);
        // Serial.print(") ");
        // Serial.print("  move: (");
        // Serial.print(vector.move_x);
        // Serial.print(", ");
        // Serial.print(vector.move_y);
        // Serial.print(",");
        // Serial.print(vector.go_ang);
        // Serial.print(", ");
        // Serial.print(vector.move_power);
        // Serial.println(") ");
        last_power=vector.move_power;
        if(vector.move_power<30){
            mymotor.free();
            uiMode=1;
            // Serial.println("no power");
        } else {
            silentTime.reset();
            // Serial.println("normal p");
            mymotor.run(vector.go_ang,vector.move_power,0);
        }
        mypixel.closest(vector.line_ang,0,255,0,1);
    }
}

void Defense::resetUI(){
    uiMode=0;
    //背景 #39c5bccc
    background.red = 57;
    background.green = 197;
    background.blue = 187;
    background.alpha = 0.8;
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
    //進行方向 #00FF00FF
    go_ang.red = 0;
    go_ang.green = 255;
    go_ang.blue = 0;
    go_ang.alpha = 1;
}

void Defense::applyUI(){
    //alphaの適用　　　
    background.red = background.red*background.alpha;
    background.green = background.green*background.alpha;
    background.blue = background.blue*background.alpha;
    P_ball.red = P_ball.red*P_ball.alpha;
    P_ball.green = P_ball.green*P_ball.alpha;
    P_ball.blue = P_ball.blue*P_ball.alpha;
    P_line.red = P_line.red*P_line.alpha;
    P_line.green = P_line.green*P_line.alpha;
    P_line.blue = P_line.blue*P_line.alpha;
    P_r_azimuth.red = P_r_azimuth.red*P_r_azimuth.alpha;
    P_r_azimuth.green = P_r_azimuth.green*P_r_azimuth.alpha;
    P_r_azimuth.blue = P_r_azimuth.blue*P_r_azimuth.alpha;
    go_ang.red = go_ang.red*go_ang.alpha;
    go_ang.green = go_ang.green*go_ang.alpha;
    go_ang.blue = go_ang.blue*go_ang.alpha;
    if(uiMode==0){
    mypixel.multi(0, 15, background.red, background.green, background.blue);
    mypixel.closest(line_azimuth,P_line.red,P_line.green,P_line.blue,1);
    mypixel.closest(ball_azimuth,P_ball.red,P_ball.green,P_ball.blue,1);
    mypixel.closest(r_azimuth,P_r_azimuth.red,P_r_azimuth.green,P_r_azimuth.blue,1);
    mypixel.closest(vector.go_ang,go_ang.red,go_ang.green,go_ang.blue,1);
    } else if(uiMode==1){
        Serial.println(silentTime.read_milli());
        // 7000msのうち何％か
        float percent = (float)silentTime.read_milli() / 7000.0;
        if(percent > 1.0) percent = 1.0;
        // 16ピクセル中何個光らせるか
        int num_pixels = (int)(percent * 16);
        Serial.print("num_pixels: "); Serial.println(num_pixels);
        mypixel.multi(0, 15, background.red, background.green, background.blue);
        if(num_pixels > 0){
            mypixel.multi(0, num_pixels-1, 255, 0, 255); // 0～num_pixels-1まで点灯
        }
    }
}

void Defense::ReadCommand(){
    mypixel.multi(0, 15, background.red, background.green, background.blue);

};

void Defense::dash(){
    silentTime.reset();
    d_timer.reset();
    while(d_timer.read_milli()<1750){
    mymotor.run(ball_azimuth,180,0);
    }
};

bool Defense::useSerial(bool use) {
    SerialEnabled = use;
    return SerialEnabled;
}
