#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

//コメントぐちゃぐちゃ



//todo
//p3戻り実装　done
//inline　　　done
//機体前めに  not yet
//ライン強化　not yet
//速度上昇　　not yet

void Defense::setup(void){//からっぽ
}

void Defense::defense_(void){
    d_timer.reset();
    gotVector=0;
    //mypixel.multi(0, 15, 0, 0, 0);
    get_vector();//センサーとか取得
    if(line_detect){// ラインが検出されている場合
        if(line_type==1){
            p1();// 水平ライン
        }
        else p3();// 垂直ライン（今は角代わり）
    }
    else{//されていなかったら戻る
        p4();//戻る
    }
}

void inline Defense::get_line_value(void){//ラインの値を取得する(p3で使うからinline)
    line.read();//ライン読み
    line_azimuth = line.get_azimuth();//ラインの方位角を取得
    line_dist = line.get_magnitude()/10;// ラインの距離を取得 /10は最大値を１１にするやつ
    line_type = line.get_type();// ラインの種類を取得
    line_detect = (line_dist == 99) ? false : true;// ラインが検出されているかどうか

    // line_his と l_s の処理を追加
    if (line_his == line_type) {//前回と同じ状態
        line_same = line_type;// ラインの種類を保存
    } else {
        l_s.reset();// ラインの種類が変わったら履歴をリセット
        line_same = 999;// ラインの種類をリセット
    }
    line_his = line_type;// ラインの種類を履歴に保存
}


void Defense::get_vector(void){//センサー取得→少し計算
    //ジャイロ(計算で使うからはじめに取得)
    r_azimuth = gam.get_azimuth();//r_azimuth robot_azimuth

    // ライン情報の取得
    //Serial.println("D_timer"+String(d_timer.read_milli()));

    //Serial.println("D_timer"+String(d_timer.read_milli()));

    get_line_value();//ラインの値を取得

    if(line_detect){//検出してたら最後の位置を記録
        if(line_azimuth<45&&line_azimuth>315) lastdetect= 0;//0に近かったら0にする
        else if(line_azimuth<225&&line_azimuth>135) lastdetect= 180;//180に近かったら180にする
        else lastdetect = line_azimuth;// それ以外はそのまま記録
    }
    //Serial.println("D_timer"+String(d_timer.read_milli()));


    //計算ぞーん
    myvector.get_cord(line_azimuth, line_dist);// 座標を計算
    line_x = myvector.get_x();// 座標のxを取得
    line_y = myvector.get_y();// 座標のyを取得

    int line_fb = 0;//line_fb line_foward/backward  1は後ろ、2は前
    if(line_azimuth<=90||line_azimuth>=270) line_fb = 1;//後ろ
    if(line_azimuth>90&&line_azimuth<270) line_fb = 2;//前
    line_go_ang = line_fb==2 ? 180 : 0;//前にあったら180、後ろにあったら0

    line_power = 200/(12-line_dist);//距離でパワーを調整　200：最大値　12-line_dist:11のときに1になるように
    // if(line_power<80) line_power = 80;//最低限80は出す
    if(line_dist<2) line_power = 0; //ラインの上にいたらパワーを0にする

    //else mypixel.closest(line_azimuth, 0, 255, 0, 3);// ラインの方位角に合わせてピクセルを点灯

    // ボール情報の取得
    //Serial.println("D_timer"+String(d_timer.read_milli()));
    ball.read();//ボール読み
    //Serial.println("D_timer"+String(d_timer.read_milli()));

    ball_azimuth = ball.get_azimuth();//ボールの方位角を取得
    // ball_absolute_angle = (ball_azimuth + r_azimuth) % 360; // 機体角度を考慮したボールの絶対角度
    ball_dist = (int)ball.get_magnitude();// ボールの距離を取得
    ball_detect = ((int)ball.get_magnitude() > 0) ? true : false;// ボールが検出されているかどうか
    myvector.get_cord(ball_azimuth, ball_dist);// 座標を計算
    ball_x = myvector.get_x();// 座標のxを取得
    ball_y = myvector.get_y();// 座標のyを取得
    //計算ぞーん
    if(ball_detect){
        ball_go_ang = (ball_x < 0) ? 270 : 90;// -だったら左に（270）、+だったら右に（90）
        ball_power=150; // ボールが検出されているときはパワーを一定にする
        if(ball_y<100) ball_power=180; // ボールが近いときはパワーを上げる
        if(ball_y<50) ball_power=200; // ボールが近いときはパワーを上げる
        ball_power = abs(ball_x)<catch_ball_X ? 0 : ball_power; // ボールとxの位置が近いときはパワーを０にする
    }
    else{
        ball_power = 0; // ボールがないときはパワーを０にする
        ball_x = 0;// ボールのx座標を０にする
        ball_go_ang = 0;// ボールの方位角を０にする
    }
    //Serial.println("D_timer"+String(d_timer.read_milli()));

    gotVector=1;// ベクトルを取得したフラグを立てる 多重取得防止用
    //debug1();//デバッグ用
    //Serial.println("D_timer"+String(d_timer.read_milli()));
    // Serial.println("l_s millo:"+String(l_s.read_milli()/1000)+"line_same:"+String(line_same));
}


void Defense::p1(void) {
    move_x = ball_power * (ball_go_ang == 90 ? 1 : -1); // 動くｘ（ボールのパワー）
    move_y = line_power * (line_go_ang == 0 ? 1 : -1); // 動くｙ（ラインのパワー）
    move_power = myvector.get_magnitude(move_x * ball_rate, move_y * line_rate); // 動くパワー（ボールのパワーとラインのパワーを合成）

    int move_azimuth = myvector.get_azimuth(move_x * ball_rate, move_y * line_rate); // 動く方位角（ボールのパワーとラインのパワーを合成）

    if (ball_power == 0) {
        p5(); // ボールがないときはラインを追跡
    } else { // 合成結果に基づいて動く
        mymotor.run(move_azimuth, move_power, 0); // 動く方位角、動くパワー*モータ倍率、０度
        // mypixel.closest(ball_go_ang, 255, 0, 0, 3); // ボールの方位角に合わせてピクセルを点灯
        // mypixel.closest(move_azimuth, 0, 255, 100, 1); // 動く方位角に合わせてピクセルを点灯
    }
}

void Defense::p2(void) {
    p3(); //　角が取得できたらこれ消す予定
    // mypixel.closest(line_azimuth, 0, 255, 0, 3);
    // int m_rl = (ball_go_ang == 90 ? 1 : -1); // move_right-left

    // int corner; // 角の分別
    // if (line_azimuth > 0 && line_azimuth < 90) {
    //     corner = 3; // 右上 キーパーライン
    // } else if (line_azimuth > 90 && line_azimuth < 180) {
    //     corner = 4; // 右下 ライン外
    // } else if (line_azimuth > 180 && line_azimuth < 270) {
    //     corner = 1; // 左下 ライン外
    // } else if (line_azimuth > 270 && line_azimuth < 360) {
    //     corner = 2; // 左上 キーパーライン
    // }

    // if (corner == 1) { // 右上
    //     if (ball_power != 0) {
    //         if (m_rl == 1) { // 右の場合 出ないようにする
    //             if (line_dist < p2_back_t) {
    //                 move_x = p2_back_s; // 動くx
    //                 move_y = line_power * (line_go_ang == 0 ? 1 : -1); // 動くｙ（ラインのパワー）
    //                 int move_azimuth = myvector.get_azimuth(move_x, move_y * line_rate); // 動く方位角（戻るパワーとラインのパワーを合成）
    //                 move_power = myvector.get_magnitude(move_x, move_y * line_rate); // 動くパワー（ボールのパワーとラインのパワーを合成）
    //                 mymotor.run(move_azimuth, move_power * motor_rate, 0); // 動く方位角、動くパワー*モータ倍率、０度
    //             } else {
    //                 mymotor.free();
    //             }
    //         } else { // 左やったらそのまま動く
    //             p1();
    //         }
    //     }
    // }

    // if (corner == 4) { // 左上
    //     if (ball_power != 0) {
    //         if (m_rl == -1) { // 左の場合 出ないようにする
    //             if (line_dist < p2_back_t) {
    //                 move_x = p2_back_s * -1; // 動くx
    //                 move_y = line_power * (line_go_ang == 0 ? 1 : -1); // 動くｙ（ラインのパワー）
    //                 int move_azimuth = myvector.get_azimuth(move_x, move_y * line_rate); // 動く方位角（戻るパワーとラインのパワーを合成）
    //                 move_power = myvector.get_magnitude(move_x, move_y * line_rate); // 動くパワー（ボールのパワーとラインのパワーを合成）
    //                 mymotor.run(move_azimuth, move_power * motor_rate, 0); // 動く方位角、動くパワー*モータ倍率、０度
    //             } else {
    //                 mymotor.free();
    //             }
    //         } else { // 右やったらそのまま動く
    //             p1();
    //         }
    //     }
    // }
}

void Defense::p3(void) { // 垂直ライン
    if (line_azimuth < 180) {//右に動く（左にある）
        p3_move(45, 135, 200); // 垂直ラインの移動処理
    } else {//左に動く（右にある）
        p3_move(315, 225, 200); // 垂直ラインの移動処理
    }
}

inline void Defense::p3_move(int dir1, int dir2, int Power) { // 垂直ラインの移動処理
    p3_timer.reset(); // 垂直ラインの移動処理用タイマーをリセット
    while (line_type != 1 && p3_timer.read_milli() < static_cast<unsigned int>(p3_move_t * 0.7)) { // 戻れたか時間切れで抜けるループ
        get_line_value(); // ラインの値を取得
        if (l_s.read_milli() > 200 && line_same == 0) { // ラインがなかったら戻る
            mybuzzer.start(330, 999); // ブザー鳴らす
            p3_timer.reset();
            while (p3_timer.read_milli() < static_cast<unsigned int>(p3_move_t)) { // ラインを検出するまでか時間切れになるまで戻り続ける
                get_line_value(); // ラインの値を取得
                if (line_detect) break; // ラインが検出されたら抜ける 戻れたってことやね
                mymotor.run(dir2, Power, 0); // キーパーラインに戻る
            }
            mybuzzer.stop(); // ブザー止める
            break; // p3から抜ける
        } else {
            mymotor.run(dir1, Power, 0); // 垂直ラインの移動処理 今は角代わり
        }
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
        }
        else {//乗ってたら何もしない
            mymotor.free();//モーター開放
        }
    }
    else{
        p4();//ラインに戻る
    }
}

void Defense::p4(void){//ラインに戻る
    mypixel.closest(lastdetect,255,0,100,3);//最後の検出角度表示　ピンク
    int backPower = 220;//戻るパワー
    mymotor.run(lastdetect,backPower,0);//最後の検出角度・戻るパワー・０度
}

void Defense::debug1(void){//デバッグ用
}

