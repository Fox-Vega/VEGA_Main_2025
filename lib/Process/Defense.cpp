#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

#define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })//Serial.printのprintfフォーマット版

void Defense::setup(void){mypixel.use_pixel(true);}

void Defense::defense_(void){
        gotVector=0;
        mypixel.multi(0, 15, 0, 0, 0);
        get_vector();
        if(line_detect){// ラインが検出されている場合
            //p1();//ボールとラインの合成
        }
        else{//されていなかったら戻る
            p4();
        }
}



void Defense::get_vector(void){
    // ライン情報の取得
    line.read();//ライン読み

    line_azimuth = line.get_azimuth();//ラインの方位角を取得
    line_dist = line.get_magnitude()/10;// ラインの距離を取得
    line_type = line.get_type();// ラインの種類を取得
    line_detect = (line_dist == 99) ? false : true;// ラインが検出されているかどうか
    myvector.get_cord(line_azimuth, line_dist);// 座標を計算
    line_x = myvector.get_x();// 座標のxを取得
    line_y = myvector.get_y();// 座標のyを取得

    if(line_detect){//検出してたら最後の位置を記録
        if(line_azimuth<45&&line_azimuth>315) lastdetect= 0;//0に近かったら0にする
        else if(line_azimuth<225&&line_azimuth>135) lastdetect= 180;// 180に近かったら180にする
        else lastdetect = line_azimuth;// それ以外はそのまま記録
    }

    //計算ぞーん
    int line_fb = 0;//line_fb line_foward/backward  1は後ろ、2は前
    if(line_azimuth<=90||line_azimuth>=270){//back
        line_fb = 1;//後ろ
    }
    if(line_azimuth>90&&line_azimuth<270){//forward
        line_fb = 2;//前
    }

    line_power = 200/(12-line_dist);//距離でパワーを調整
    if(line_power<60) line_power = 60;//最低限60は出す
    line_go_ang = line_fb==2 ? 180 : 0;//前にあったら180、後ろにあったら0
    if(line_dist<2) line_power = 0; //ラインの上にいたらパワーを0にする
    else mypixel.closest(line_azimuth, 0, 255, 0, 3);// ラインの方位角に合わせてピクセルを点灯

    // ボール情報の取得
    ball.read();//ボール読み

    ball_azimuth = ball.get_azimuth();//ボールの方位角を取得
    ball_dist = (int)ball.get_magnitude();// ボールの距離を取得
    ball_detect = ((int)ball.get_magnitude() > 0) ? true : false;// ボールが検出されているかどうか
    myvector.get_cord(ball_azimuth, ball_dist);// 座標を計算
    ball_x = myvector.get_x();// 座標のxを取得
    Serial.println(ball_x);
    ball_y = myvector.get_y();// 座標のyを取得
    //計算ぞーん
    if(ball_detect){
        ball_x= ball_x> ball_max_X ? ball_max_X : ball_x;// ボールの位置が最大値を超えたら最大値にする
        ball_power = (ball_x/30)*30; // ボールの位置に応じてパワーを調整　  ボールｘ/30　+　20（前後で１か-1掛ける）
        ball_power = abs(ball_x)<catch_ball_X ? 0 : ball_power; // ボールとxの位置が近いときはパワーを０にする
        ball_go_ang = (ball_x < 0) ? 270 : 90;// -だったら左に（270）、+だったら右に（90）
    }
    else{
        ball_power = 0; // ボールがないときはパワーを０にする
        ball_x = 0;// ボールのx座標を０にする
        ball_go_ang = 0;// ボールの方位角を０にする
    }

    //ジャイロ
    r_azimuth = gam.get_azimuth();//r_azimuth robot_azimuth

    gotVector=1;// ベクトルを取得したフラグを立てる 多重取得防止用
    debug1();//デバッグ用
}


void Defense::p1(void){
    move_x=ball_power;//動くｘ（ボールのパワー）
    move_y=line_power*(line_go_ang==0 ? 1 : -1);//動くｙ（ラインのパワー）

    move_power = myvector.get_magnitude(move_x*ball_rate, move_y*line_rate);//動くパワー（ボールのパワーとラインのパワーを合成）
        int move_azimuth = myvector.get_azimuth(move_x*ball_rate, move_y*line_rate);//動く方位角（ボールのパワーとラインのパワーを合成）
        if(ball_power==0)trace();// ボールがないときはラインを追跡
        else if(line_dist<2) {//ライン上にいたらボールだけを追う
            mymotor.run(ball_go_ang,ball_power*ball_rate,0);//ボールの方位角、ボールのパワー*ボール倍率、０度
            mypixel.closest(ball_go_ang, 255, 0, 0, 3);// ボールの方位角に合わせてピクセルを点灯
            printf_s("a");
            Serial.println(move_x);
        }
        else {//合成結果に基づいて動く
            mymotor.run(move_azimuth, move_power*motor_rate,0);//動く方位角、動くパワー*モータ倍率、０度
            mypixel.closest(move_azimuth, 0, 255, 0, 3);// 動く方位角に合わせてピクセルを点灯
            printf_s("a");
            Serial.println(move_x);
        }
}

// void Defense::p2(void){
//     int line_azimuth_mod[2] = {999,999};
//     if(line_azimuth>0&&line_azimuth<90)
//     else if(line_azimuth>90&&line_azimuth<180)
//     else if(line_azimuth>180&&line_azimuth<270)
//     else if(line_azimuth>270&&line_azimuth<360)
//     else line_azimuth_mod = 999;
//     if(line_azimuth_mod==999)p4();
//     else{

//     }
// }

void Defense::trace(void){//ラインを追跡
    if(gotVector==0) get_vector();//多重取得防止
    if(line_detect){// ラインが検出されている場合
        if(line_dist>2){//ラインに戻ろうと動く
            mymotor.run(line_go_ang, line_power*line_rate*motor_rate, 0);//ラインの向き・パワー*ライン倍率・０度
        }
        else if(true!=(r_azimuth<15&&r_azimuth>345)){//向きがズレすぎてた場合回転
            mymotor.run(0,0,0);//姿勢制御

            int d=  0-r_azimuth;//ピクセルに０度を表示
            if(d<0) d+=360;//０度未満だったら３６０を足すe
            mypixel.closest(d, 255, 255, 255, 3);// ピクセルを白色にして０度を表示
        }
        else {//乗ってたら何もしない
            mymotor.free();//自由回転
            mypixel.multi(0, 15, 0, 0, 255);// ピクセルを緑色にして０度を表示
        }
    }
    else{
        p4();//ラインに戻る
    }
}

void Defense::p4(void){//ラインに戻る
    mypixel.closest(lastdetect,255,0,0,3);
    int backPower =(lastdetect<15&&lastdetect>345)? 250 : 200;//ラインよりも後ろにいたら早く戻るようにする
    mymotor.run(lastdetect,backPower,0);//最後の検出角度・戻るパワー・０度
    printf_s("noline:%d\n", lastdetect);//ラインがないですよ～　最後の検出角度
}

void Defense::debug1(void){//デバッグ用
    printf_s("\n>debug1-line_azimuth:%d\n", line_azimuth);//ラインの方位角
    printf_s(">debug1-R_azimuth:%d\n", r_azimuth);//ロボットの方位角
    printf_s(">debug1-line_type:%d\n", line_type);//ラインの種類
    printf_s(">debug1-line_dist:%d\n", line_dist);//ラインの距離
    printf_s(">debug1-line_x:%f\n", (double)line_x);//ラインのx座標
    printf_s(">debug1-line_y:%f\n", (double)line_y);//ラインのy座標
    printf_s(">debug1-ball_azimuth:%d\n", ball_azimuth);//ボールの方位角
    printf_s(">debug1-ball_dist:%d\n", ball_dist);//ボールの距離
    printf_s(">debug1-ball_x:%d\n", ball_x);//ボールのx座標
    printf_s(">debug1-ball_y:%f\n", ball_y);//ボールのy座標
}

