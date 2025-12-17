#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"

///@brief　そゆこと
#define diff(a) a/abs(a)

// Timer変数の実体定義
Timer SilentTime,MoveTime;
int LastReturn = 0;

// 復帰方向リストの定義
const int back_ang[4] = {180, 180, 225, 135};

// グローバル変数の定義（Defense.cppでのみ定義）
static int lastdetect =0 ;

//-----調整用定数-----//

///　@brief ダッシュ待ち時間
const int dash_border = 12000;
/// @brief 基本移動速度
const int move_speed = 200;
/// @brief 最小移動速度
const int move_border = 30;
/// @brief ボール補正角度
const int ball_cal =20;
//// @brief ダッシュ時間
const unsigned int dash_time = 2500;
/// @brief ボール移動境界(±角度)
const float ball_move_border = 4;
/// @brief ノイズ除去
const int noise_border = 400;
/// @brief ライン強化
const int line_back_mag=7;

//処理速度
int dhst;
Timer Dhs;

/// @brief 初期化
void Defense::setup(void){
    reset();
}

void Defense::defense_(int start_cord){
    Dhs.reset();
    mypixel.use_pixel(true);
    if(start_cord != 0) {
        int aaa = back_ang[start_cord-1];
        mybuzzer.start(500,999);
        delay(100);
        while(!(line.get_type() == 2)) {
            line.read();
            ball.read();
            gam.read_azimuth();
            mymotor.run(aaa, 120, 0);
            if(myswitch.check_toggle() == 0) {
                return;
            }
        }
        mymotor.run(0,200,0);
        delay(100);
        return;
    }

    int line_azimuth,line_type,ball_azimuth,ball_stat,gam_azimuth,line_mag;
    float move_power,move_x,move_y,ball_x,ball_y,line_x,line_y= 0.0f;
    int calc_move_speed,ball_ang,move_azimuth = 0;
    line_azimuth=line.get_azimuth();
    line_type=line.get_type();
    line_mag=line.get_magnitude();
    ball_azimuth=ball.get_azimuth();
    ball_stat=ball.get_stat();
    gam_azimuth=gam.get_azimuth();

    bool tl =(line.get_stat(0) || line.get_stat(1) || line.get_stat(2) ||line.get_stat(23) || line.get_stat(22))&&
    (line.get_stat(11) || line.get_stat(12) || line.get_stat(13) || line.get_stat(10) || line.get_stat(9));
    bool corner =(line.get_type()==2&&(getErr(line.get_pack(0),line.get_pack(1))<110));

    // if(SilentTime.read_milli()>(unsigned long)dash_border){dash();return;}
    if(line_type==0){
        SilentTime.reset();
        mybuzzer.start(1500,999);
        mymotor.run(lastdetect,200,0);
        return;
    }if(line_type==3){
        SilentTime.reset();
        mymotor.run(0,200,0);
        return;
    }if(ball_stat==false){
        mymotor.free();
        mybuzzer.start(250,999);
        SilentTime.reset();
        return;
    }
    mybuzzer.stop();
    lastdetect=line_azimuth;
    applyXY(line_azimuth, line_x, line_y);
    ball_ang = ball_azimuth + ball_cal;
    ball_x = ball_ang < 180 ? 1 : -1;
    ball_y = isFront(ball_ang) ? 1 : -1;
    calc_move_speed=tl ? move_speed>>1 : move_speed;
    line_x = corner ? line_x * 1.8 : line_x;
    line_y = corner ? line_y * 1.8 : line_y;
    ball_x = tl ? 0 : ball_x;
    line_x = !tl ? 0 : line_x;
    ball_y = !tl ? 0 : ball_y;
    move_x = (line_x + ball_x) * calc_move_speed;
    move_y = (line_y + ball_y) * calc_move_speed;

    if((tl&&isFront(ball_azimuth))){
        move_x = diff(line_x) * move_speed;
        move_y = move_speed;
        mypixel.multi(0, 15, 0, 0, 255);
    }

    move_azimuth = norm360(myvector.get_azimuth(move_x, move_y));
    move_power = myvector.get_magnitude(abs(move_x), abs(move_y));
    if(line_mag > line_back_mag){
        mypixel.use_pixel(true);
        mypixel.closest(line_azimuth, 0, 255, 0, 1);
        move_azimuth = line_azimuth;
        calc_move_speed = 255;
    }

    move_power=getErr(0, ball_azimuth) < ball_move_border && (!tl) ? 0 : move_power;

    if(move_power > move_border && (!tl || (tl && isFront(ball_azimuth)))){
        mymotor.run(move_azimuth - gam_azimuth, (int) move_power, 0);
        if(MoveTime.read_milli() > (unsigned int) noise_border) SilentTime.reset();
        mypixel.closest(move_azimuth, 255, 255, 255, 1);
    }else{
        if(tl)SilentTime.reset();
        MoveTime.reset();
        mymotor.free();
    }
    dhst=Dhs.read_milli();
    //mybuzzer.start((int)scaleRange(0.0f, dash_border, 500.0f, 1500.0f, (float)SilentTime.read_milli()), 999);
}

int Defense::dhstget(void){return dhst;}

void Defense::reset(void){
    MoveTime.reset();
    SilentTime.reset();
}

void Defense::dash(void){
    MoveTime.reset();
    float TL = 20.0;
    float TLM = 60.0;
    if(SilentTime.read_milli() < dash_border * 1.2) {
        if(myswitch.check_toggle() == 0) {
            SilentTime.reset();
            return;
        }

        mypixel.multi(0, 15, 255, 50, 50);
        mypixel.show();
        SilentTime.reset();
        
        int dash_start = millis();
        while((millis() - dash_start) < 750){
            gam.read_azimuth();
            mymotor.run(0, 220, 0);
        }
        mymotor.run(line.get_eazimuth(),220,0);
        delay(500);
        dash_start = millis();
        ball.read();
        while((millis() - dash_start) < dash_time) {
            gam.read_azimuth();
            line.read();
            mymotor.run(ball.get_azimuth(), 220, 0);
            if(myswitch.check_toggle() == 0 || !isFront(ball.get_azimuth())) {
                SilentTime.reset();
                break;
            }
        }
        SilentTime.reset();
        mymotor.free();

        delay(10);


        int mm = 180;
        int mt = 75;
        while(1) {
            gam.read_azimuth();
            ball.read();
            line.read();
            if(line.get_type() == 1) {
                if((line.get_azimuth() < 180 - TL && line.get_azimuth() > TL) ||
                    (line.get_azimuth() > 180 + TL && line.get_azimuth() < 360 - TL)) {
                    if(line.get_azimuth() < 180 - TL && line.get_azimuth() > TL) {
                        mm = 180 - TLM;
                    } else {
                        mm = 180 + TLM;
                    }
                }
                break;
            }
            mymotor.run(mm, static_cast<int>(mt), 0);
            if(mt < 120) mt += 1;
            if(myswitch.check_toggle() == 0) {
                SilentTime.reset();
                break;
            }
        }
        mymotor.free();
        delay(100);
    } else {
        SilentTime.reset();
    }
}