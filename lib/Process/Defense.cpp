#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

//todo
//機体前めに  not yet
//縦ラインもやりたい　今角代わりやけど角は特定できたから
//角の処理は　外側(右側だったら右下ってこと)だったら距離が大きければ戻る　それ以外はいまのままでいい外側をとにかく



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
    d_timer.reset();
    if(SerialEnabled){

    }
    get_value();
}

void Defense::get_value(){
    line.read();
    ball.read();

    line_detect=line.get_magnitude()==999 ? false : true;
    if(line_detect){
        line_azimuth=line.get_azimuth();
        line_absolute_angle=norm360(line_azimuth - r_azimuth);
        line_dist=line.get_magnitude();
    } else {
        line_azimuth=999;
        line_absolute_angle=999;
        line_dist=999;
        line_x=999;
        line_y=999;
    }

    //ball_detect=true;
    ball_azimuth=ball.get_azimuth();
    ball_a
    ball_dist=ball.get_magnitude();
}

bool Defense::useSerial(bool use) {
    SerialEnabled = use;
    return SerialEnabled;
}

// // void Defense::defense_(void){
// //     d_timer.reset();
// //     if(useTimer){Serial.println("before:" + String(d_timer.read_milli()));}
// //     gotVector=0;
// //     get_vector();//センサーとか取得
// //     if(useTimer){Serial.println("after get vector:" + String(d_timer.read_milli()));}
// //     if(line_detect){// ラインが検出されている場合
// //         if(line_type==1){
// //             p1();// 水平ライン
// //             if(useTimer){Serial.println("after p1:" + String(d_timer.read_milli()));}
// //         }
// //         else if(line_type==2){
// //             p2();// 角ライン
// //             if(useTimer){Serial.println("after p2:" + String(d_timer.read_milli()));}
// //         }
// //         else if(line_type==3){
// //             p3();// 垂直ライン
// //             if(useTimer){Serial.println("after p3:" + String(d_timer.read_milli()));}
// //         }
// //     }
// //     else{//されていなかったら戻る
// //         p4();//戻る
// //         if(useTimer){Serial.println("after p4:" + String(d_timer.read_milli()));}
// //     }
// //     if(!useMotor){
// //         mymotor.free();
// //         if(useTimer){Serial.println("usemotor false:" + String(d_timer.read_milli()));}
// //     }
// //     if(useTimer){Serial.println("after all:" + String(d_timer.read_milli()));}
// // }

// // void inline Defense::get_line_value(void){//ラインの値を取得する(p3で使うからinline)
// //     line.read();//ライン読み
// //     line_azimuth = line.get_azimuth();//ラインの方位角を取得
// //     line_dist = line.get_magnitude()/10;// ラインの距離を取得 /10は最大値を１１にするやつ
// //     line_type = line.get_type();// ラインの種類を取得
// //     line_detect = (line_dist == 99) ? false : true;// ラインが検出されているかどうか

// //     // line_his と l_s の処理を追加
// //     if (line_his == line_type) {//前回と同じ状態
// //         line_same = line_type;// ラインの種類を保存
// //     } else {
// //         l_s.reset();// ラインの種類が変わったら履歴をリセット
// //         line_same = 999;// ラインの種類をリセット
// //     }
// //     line_his = line_type;// ラインの種類を履歴に保存
// // }


// // void Defense::get_vector(void){//センサー取得→少し計算
// //     //ジャイロ(計算で使うからはじめに取得)
// //     r_azimuth = gam.get_azimuth();//r_azimuth robot_azimuth
// //     get_line_value();//ラインの値を取得
// //     if(line_detect){//検出してたら最後の位置を記録
// //         if(line_azimuth<45&&line_azimuth>315) lastdetect= 0;//0に近かったら0にする
// //         else if(line_azimuth<225&&line_azimuth>135) lastdetect= 180;//180に近かったら180にする
// //         else lastdetect = line_azimuth;// それ以外はそのまま記録
// //     }
// //     //計算ぞーん
// //     myvector.get_cord(line_azimuth, line_dist);// 座標を計算
// //     line_x = myvector.get_x();// 座標のxを取得
// //     line_y = myvector.get_y();// 座標のyを取得
// //     if(line_type!=2){
// //     int line_absorate_azimuth = line_azimuth - r_azimuth;
// //     // ここから方位(0/45/90/…/315)判定  ±15°許容
// //     int la = line_absorate_azimuth % 360; if(la < 0) la += 360; // 正規化
// //     line_sector = -1; // 0,45,90,...,315 / -1=該当なし
// //     if( la >= 30  && la <= 60  ) line_sector = 45;            // 右上
// //     else if( la >= 120 && la <= 150 ) line_sector = 135;           // 右下
// //     else if( la >= 210 && la <= 240 ) line_sector = 225;           // 左下
// //     else if( la >= 300 && la <= 330 ) line_sector = 315;           // 左上
// //     if(line_sector!=-1)line_type= 2;}
// //     else {line_azimuth=line_sector;}
// //     // ここまで
// //     int line_fb = 0;//line_fb line_foward/backward  1は後ろ、2は前
// //     if(line_azimuth<=90||line_azimuth>=270) line_fb = 1;//前
// //     else if(line_azimuth>90&&line_azimuth<270) line_fb = 2;//後ろ
// //     line_go_ang = line_fb==2 ? 180 : 0;//前にあったら180、後ろにあったら0
// //     line_power = 255/(12-line_dist);//距離でパワーを調整　200：最大値　12-line_dist:11のときに1になるように
// //     if(line_dist<2) line_power = 0; //ラインの上にいたらパワーを0にする


// //     ball.read();//ボール読み
// //     ball_azimuth = ball.get_azimuth();//ボールの方位角を取得
// //     ball_dist = (int)ball.get_magnitude();// ボールの距離を取得
// //     ball_detect = ((int)ball.get_magnitude() > 0) ? true : false;// ボールが検出されているかどうか
// //     myvector.get_cord(ball_azimuth, ball_dist);// 座標を計算
// //     ball_x = myvector.get_x();// 座標のxを取得
// //     ball_y = myvector.get_y();// 座標のyを取得
// //     //計算ぞーん
// //     if(ball_detect){
// //         ball_go_ang = (ball_x < 0) ? 270 : 90;// -だったら左に（270）、+だったら右に（90）
// //         ball_power=150; // ボールが検出されているときはパワーを一定にする
// //         if(ball_y<120) ball_power=180; // ボールが近いときはパワーを上げる
// //         if(ball_y<70) ball_power=200; // ボールが近いときはパワーを上げる
// //         ball_power = abs(ball_x)<catch_ball_X ? 0 : ball_power; // ボールとxの位置が近いときはパワーを０にする
// //     }
// //     else{
// //         ball_power = 0; // ボールがないときはパワーを０にする
// //         ball_x = 0;// ボールのx座標を０にする
// //         ball_go_ang = 0;// ボールの方位角を０にする
// //     }

// //     gotVector=1;// ベクトルを取得したフラグを立てる 多重取得防止用
// // }




// // void Defense::p1(void) {
// //     if (ball_power == 0) {
// //         p5(); // ボールがないときはラインを追跡
// //     } else { // 合成結果に基づいて動く
// //         move_x = ball_power * (ball_go_ang == 90 ? 1 : -1); // 動くｘ（ボールのパワー）
// //         move_y = line_power * (line_go_ang == 0 ? 1 : -1); // 動くｙ（ラインのパワー）
// //         move_power = myvector.get_magnitude(move_x * ball_rate, move_y * line_rate); // 動くパワー（ボールのパワーとラインのパワーを合成）
// //         move_azimuth = myvector.get_azimuth(move_x * ball_rate, move_y * line_rate); // 動く方位角（ボールのパワーとラインのパワーを合成）
// //         if(useMotor) mymotor.run(move_azimuth, move_power, 0); // 動く方位角、動くパワー*モータ倍率、０度
// //     }
// // }

// // void Defense::p2(void) {
// //     mypixel.closest(line_azimuth, 0, 255, 0, 3);
// //     int m_rl = (ball_go_ang == 90 ? 1 : -1); // move_right-left

// //     int corner; // 角の分別
// //     if (line_azimuth > 0 && line_azimuth < 90) {
// //         corner = 1; // 右上
// //     } else if (line_azimuth > 90 && line_azimuth < 180) {
// //         corner = 3; // 右下
// //     } else if (line_azimuth > 180 && line_azimuth < 270) {
// //         corner = 2; // 左下
// //     } else if (line_azimuth > 270 && line_azimuth < 360) {
// //         corner = 4; // 左上
// //     }

// //     if (corner == 1) { // 右上
// //         if (ball_power != 0) {
// //             if (m_rl == 1) { // 右の場合 出ないようにする
// //                 if (line_dist < p2_back_t) {
// //                     move_x = p2_back_s; // 動くx
// //                     move_y = line_power * (line_go_ang == 0 ? 1 : -1); // 動くｙ（ラインのパワー）
// //                     int move_azimuth = myvector.get_azimuth(move_x, move_y * line_rate); // 動く方位角（戻るパワーとラインのパワーを合成）
// //                     move_power = myvector.get_magnitude(move_x, move_y * line_rate); // 動くパワー（ボールのパワーとラインのパワーを合成）
// //                     if(useMotor) mymotor.run(move_azimuth, move_power * motor_rate, 0); // 動く方位角、動くパワー*モータ倍率、０度
// //                 } else {
// //                     if(useMotor) mymotor.free(); // モーター開放
// //                 }
// //             } else { // 左やったらそのまま動く
// //                 p1();
// //             }
// //         }
// //     }

// //     if (corner == 4) { // 左上
// //         if (ball_power != 0) {
// //             if (m_rl == -1) { // 左の場合 出ないようにする
// //                 if (line_dist < p2_back_t) {
// //                     move_x = p2_back_s * -1; // 動くx
// //                     move_y = line_power * (line_go_ang == 0 ? 1 : -1); // 動くｙ（ラインのパワー）
// //                     int move_azimuth = myvector.get_azimuth(move_x, move_y * line_rate); // 動く方位角（戻るパワーとラインのパワーを合成）
// //                     move_power = myvector.get_magnitude(move_x, move_y * line_rate); // 動くパワー（ボールのパワーとラインのパワーを合成）
// //                     if(useMotor) mymotor.run(move_azimuth, move_power * motor_rate, 0); // 動く方位角、動くパワー*モータ倍率、０度
// //                 } else {
// //                     if(useMotor) mymotor.free(); // モーター開放
// //                 }
// //             } else { // 右やったらそのまま動く
// //                 p1();
// //             }
// //         }
// //     }

// //     if(corner == 2){//右下
// //         if(line_dist>p2_back_t){
// //             mymotor.run(225,p2_back_s,0);
// //         } else {
// //             if(ball_go_ang==270){
// //                 line_fb=1;
// //                 p1();
// //             } else {
// //                 mymotor.free();
// //             }
// //         }
// //     }

// //     if(corner == 3){//左下
// //         if(line_dist>p2_back_t){
// //             mymotor.run(135,p2_back_s,0);
// //         } else {
// //             if(ball_go_ang==90){
// //                 line_fb=1;
// //                 p1();
// //             } else {
// //                 mymotor.free();
// //             }
// //         }
// //     }
// // }

// // void Defense::p3(void) { // 垂直ライン
// //     mymotor.run(0,100,0);
// //     // if (line_azimuth < 180) {//右に動く（左にある）
// //     //     p3_move(45, 135, 200); // 垂直ラインの移動処理
// //     // } else {//左に動く（右にある）
// //     //     p3_move(315, 225, 200); // 垂直ラインの移動処理
// //     // }
// // }

// // inline void Defense::p3_move(int dir1, int dir2, int Power) {
// //     // p3_timer.reset(); // タイマーをリセット
// //     // while (line_type != 1 && p3_timer.read_milli() < static_cast<unsigned int>(p3_move_t * 0.7)) {
// //     //     get_line_value(); // ラインの値を取得
// //     //     if (l_s.read_milli() > 200 && line_same == 0) { // ラインがなかった場合
// //     //         mybuzzer.start(330, 999); // ブザーを鳴らす
// //     //         p3_timer.reset();
// //     //         while (p3_timer.read_milli() < static_cast<unsigned int>(p3_move_t >> 1) && !line_detect) {
// //     //             get_line_value(); // ラインの値を取得
// //     //             if(useMotor) mymotor.run(dir2, Power, 0); // キーパーラインに戻る
// //     //         }
// //     //         mybuzzer.stop(); // ブザーを止める
// //     //         break;
// //     //     }
// //     //     if(useMotor) mymotor.run(dir1, Power, 0); // 垂直ラインの移動処理
// //     // }
// //     mymotor.run(0,100,0);
// // }


// // void Defense::p5(void){//ラインを追跡
// //     if(line_detect){// ラインが検出されている場合
// //         if(line_dist>p5_t){//ラインに戻ろうと動く
// //             if(useMotor) mymotor.run(line_go_ang, line_power*motor_rate, 0);//ラインの向き・パワー*ライン倍率・０度
// //         }
// //         else {//乗ってたら何もしない
// //             if(!(r_azimuth<15&&r_azimuth>345)){//向きがズレすぎてた場合回転
// //                 if(useMotor) mymotor.run(0,0,0);//0度に向く
// //                 int d=  0-r_azimuth;//ピクセルに０度を表示
// //                 if(d<0) d+=360;//０度未満だったら３６０を足す
// //             }else{
// //                 if(useMotor) mymotor.free();//モーター開放
// //             }
// //         }
// //     }
// //     else{
// //         p4();//ラインに戻る
// //     }
// // }

// // void Defense::p4(void){//ラインに戻る
// //     int backPower = 220;//戻るパワー
// //     if(useMotor) mymotor.run(lastdetect,backPower,0);//最後の検出角度・戻るパワー・０度
// // }

// // void Defense::debug1(void){//デバッグ用 
// // }

