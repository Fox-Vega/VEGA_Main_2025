#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"


// ネオピク
// 緑             ライン
// 赤             ボールの右左
// 白             ボールとｘが同じ
// ピンク         最後の検出角度
// 青緑っぽいやつ(緑強めの)  動く角度

//コメントぐちゃぐちゃ


void Defense::setup(void){//からっぽ
}

void Defense::defense_(void){
        gotVector=0;
        //mypixel.multi(0, 15, 0, 0, 0);
        get_vector();//センサーとか取得
        if(line_detect){// ラインが検出されている場合
            p1();//動く
        }
        else{//されていなかったら戻る
            p4();//戻る
        }
}



void Defense::get_vector(void){//センサー取得→少し計算
    // ライン情報の取得
    line.read();//ライン読み

    line_azimuth = line.get_azimuth();//ラインの方位角を取得
    line_dist = line.get_magnitude()/10;// ラインの距離を取得 /10は最大値を１１にするやつ
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
    if(line_azimuth<=90||line_azimuth>=270) line_fb = 1;//後ろ
    if(line_azimuth>90&&line_azimuth<270) line_fb = 2;//前
    line_go_ang = line_fb==2 ? 180 : 0;//前にあったら180、後ろにあったら0

    line_power = 200/(12-line_dist);//距離でパワーを調整　200：最大値　12-line_dist:11のときに1になるように
    if(line_power<60) line_power = 60;//最低限60は出す
    if(line_dist<2) line_power = 0; //ラインの上にいたらパワーを0にする

    //else mypixel.closest(line_azimuth, 0, 255, 0, 3);// ラインの方位角に合わせてピクセルを点灯

    // ボール情報の取得
    ball.read();//ボール読み

    ball_azimuth = ball.get_azimuth();//ボールの方位角を取得
    ball_dist = (int)ball.get_magnitude();// ボールの距離を取得
    ball_detect = ((int)ball.get_magnitude() > 0) ? true : false;// ボールが検出されているかどうか
    myvector.get_cord(ball_azimuth, ball_dist);// 座標を計算
    ball_x = myvector.get_x();// 座標のxを取得
    ball_y = myvector.get_y();// 座標のyを取得
    //計算ぞーん
    if(ball_detect){
        ball_x= ball_x> ball_max_X ? ball_max_X : ball_x;// ボールの位置が最大値を超えたら最大値にする
        ball_power = ball_x;//後で動的に調整する　ーーー
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
        if(ball_power==0)p5();// ボールがないときはラインを追跡
        else if(line_dist<2) {//ライン上にいたらボールだけを追う
            mymotor.run(ball_go_ang,ball_power*ball_rate,0);//ボールの方位角、ボールのパワー*ボール倍率、０度
            //mypixel.closest(ball_go_ang, 255, 0, 0, 3);// ボールの方位角に合わせてピクセルを点灯
        }
        else {//合成結果に基づいて動く
            mymotor.run(move_azimuth, move_power*motor_rate,0);//動く方位角、動くパワー*モータ倍率、０度
            //mypixel.closest(ball_go_ang, 255, 0, 0, 3);// ボールの方位角に合わせてピクセルを点灯
            //mypixel.closest(move_azimuth, 0, 255, 100, 1);// 動く方位角に合わせてピクセルを点灯
        }
}

void Defense::p5(void){//ラインを追跡
    if(line_detect){// ラインが検出されている場合
        if(line_dist>2){//ラインに戻ろうと動く
            mymotor.run(line_go_ang, line_power*line_rate*motor_rate, 0);//ラインの向き・パワー*ライン倍率・０度
        }
        else if(true!=(r_azimuth<15&&r_azimuth>345)){//向きがズレすぎてた場合回転
            mymotor.run(0,0,0);//0度に向く
            int d=  0-r_azimuth;//ピクセルに０度を表示
            if(d<0) d+=360;//０度未満だったら３６０を足す
            //mypixel.closest(d, 100, 50, 255, 3);//
        }
        else {//乗ってたら何もしない
            mymotor.free();//モーター開放
            //mypixel.multi(0, 15, 0, 0, 255);// ピクセルを緑色にして０度を表示
        }
    }
    else{
        p4();//ラインに戻る
    }
}

void Defense::p4(void){//ラインに戻る
    //mypixel.closest(lastdetect,255,0,100,3);//最後の検出角度表示　ピンク
    int backPower = 220;//戻るパワー
    mymotor.run(lastdetect,backPower,0);//最後の検出角度・戻るパワー・０度
}

void Defense::debug1(void){//デバッグ用
    // Serial.println(">debug1-line_azimuth:" + String(line_azimuth));//ラインの方位角
    // Serial.println(">debug1-R_azimuth:" + String(r_azimuth));//ロボットの方位角
    // Serial.println(">debug1-line_type:" + String(line_type));//ラインの種類
    // Serial.println(">debug1-line_dist:" + String(line_dist));//ラインの距離
    // Serial.println(">debug1-line_x:" + String(line_x));//ラインのx座標
    // Serial.println(">debug1-line_y:" + String(line_y));//ラインのy座標
    // Serial.println(">debug1-ball_azimuth:" + String(ball_azimuth));//ボールの方位角
    // Serial.println(">debug1-ball_dist:" + String(ball_dist));//ボールの距離
    // Serial.println(">debug1-ball_x:" + String(ball_x));//ボールのx座標
    // Serial.println(">debug1-ball_y:" + String(ball_y));//ボールのy座標
    // //line_power
    // Serial.println(">debug1-line_power:" + String(line_power));//ラインのパワー
    // Serial.println(">debug1-ball_power:" + String(ball_power));//ボールのパワー
    // Serial.println(">debug1-line_go_ang:" + String(line_go_ang));//ラインの進行方向
    // Serial.println(">debug1-ball_go_ang:" + String(ball_go_ang));//ボールの進行方向


    // Serial.println(">debug1-move_x:" + String(move_x));//動くx
    // Serial.println(">debug1-move_y:" + String(move_y));//動くy
    // Serial.println(">debug1-move_power:" + String(move_power));//動くパワー
    // Serial.println(">debug1-lastdetect:" + String(lastdetect));//最後の検出角度
    // Serial.println(">debug1-gotVector:" + String(gotVector));//ベクトル
    // Serial.println(">debug1-r_azimuth:" + String(r_azimuth));//ロボットの方位角
    // Serial.println(">debug1-ball_detect:" + String(ball_detect));//ボールが検出7
    Serial.println(">debug1-line_detect:" + String(line_detect));//ラインが検出
}

