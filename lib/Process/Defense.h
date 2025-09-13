#pragma once
#include <Arduino.h>
#include <Timer.h>

// ちなpはプロセスの略

class Defense {
public:
    void setup(void);                  // 初期設定
    void defense_(void);               // メイン処置
    // bool useSerial(bool use);          // シリアルを使うかどうか コマンドで変える
    // void timerReset();
private:
    const int dash_border=8000;
    const int ball_move_border=15;
    const int ball_power=180;//ball
    const float line_late=0.7;

    int lastdetect;
    int move_azimuth;
    int move_power;
    double move_x;
    double move_y;
    bool ball_move=false;
    Timer Dtimer;
    // モード定義
    // bool SerialEnabled = false;        // シリアル通信の有効/無効
    // bool PixelEnabled = true;
    // // --- 調整用定数 ---
    // float line_late = 1;
    // int dashTime = 1500;
    // int line_max =  10000;

    // --- ボール関連変数 ---
    // bool ball_detect;                  // ボールが検出されているかどうか
    // int ball_azimuth;                  // ボールの方位角
    // int ball_absolute_angle;           // ボールの絶対角度
    // int ball_dist;                     // ボールの距離
    // int ball_x;                        // ボールのx座標
    // int ball_y;                        // ボールのy座標

    // --- ライン関連変数 ---
    // bool line_detect;                  // ラインが検出されているかどうか
    // int line_azimuth;                  // ラインの方位角
    // int line_absolute_angle;           // ラインの絶対角度
    // int line_dist;                     // ラインの距離
    // int lastdetect;                    // 最後に検出したラインの方位角
    // int line_x;                        // ラインのx座標
    // int line_y;                        // ラインのy座標

    // --- ベクトル ---
    // struct vector {
    //     int ball_ang;      // ボールの動きたい方向
    //     int ball_power;    // ボールのパワー
    //     int ball_x;        // ボールのx座標
    //     int ball_y;        // ボールのy座標
    //     int line_ang;      // ラインの動きたい方向
    //     int line_power;    // ラインのパワー
    //     int line_x;        // ラインのx座標
    //     int line_y;        // ラインのy座標
    //     int go_ang;        // 動く方向
    //     int move_power;    // 動く強さ
    //     int move_x;        // 動くx
    //     int move_y;        // 動くy
    // };
    // vector vector;

    // --- NeoPixel ---
    // struct RGBA {
    //     int red;
    //     int green;
    //     int blue;
    //     float alpha;
    // };
    // RGBA background;
    // RGBA P_line;
    // RGBA P_ball;
    // RGBA P_r_azimuth;
    // RGBA go_ang;

    // --- その他処理用変数 ---
    // int mode;
    // int last_power;
    // int r_azimuth;
    // Timer d_timer;
    // Timer silentTime;
    // int uiMode = 0;
    // bool IsDash = false;

    // --- メソッド ---
    // void get_value();
    // int cal_vector();
    // void move();
    // void dash();
    // void back();
    // void ReadCommand();
    // void resetUI();
    // void applyUI();
    // inline int norm360(int a) { a %= 360; if(a < 0) a += 360; return a; }
    // inline bool diff_signs(int a, int b) {return (a >= 0 && b < 0) || (a < 0 && b >= 0);}


    // 前ディフェンス
    // void Defense::defense_(void) {
    //     d_timer.reset();
    //     if(useTimer) { Serial.println("before:" + String(d_timer.read_milli())); }
    //     gotVector = 0;
    //     get_vector(); // センサーとか取得
    //     if(useTimer) { Serial.println("after get vector:" + String(d_timer.read_milli())); }
    //     if(line_detect) {
    //         if(line_type == 1) {
    //             p1(); // 水平ライン
    //             if(useTimer) { Serial.println("after p1:" + String(d_timer.read_milli())); }
    //         } else if(line_type == 2) {
    //             p2(); // 角ライン
    //             if(useTimer) { Serial.println("after p2:" + String(d_timer.read_milli())); }
    //         } else if(line_type == 3) {
    //             p3(); // 垂直ライン
    //             if(useTimer) { Serial.println("after p3:" + String(d_timer.read_milli())); }
    //         }
    //     } else {
    //         p4(); // 戻る
    //         if(useTimer) { Serial.println("after p4:" + String(d_timer.read_milli())); }
    //     }
    //     if(!useMotor) {
    //         mymotor.free();
    //         if(useTimer) { Serial.println("usemotor false:" + String(d_timer.read_milli())); }
    //     }
    //     if(useTimer) { Serial.println("after all:" + String(d_timer.read_milli())); }
    // }

    // void inline Defense::get_line_value(void) {
    //     line.read();
    //     line_azimuth = line.get_azimuth();
    //     line_dist = line.get_magnitude() / 10;
    //     line_type = line.get_type();
    //     line_detect = (line_dist == 99) ? false : true;
    //     if (line_his == line_type) {
    //         line_same = line_type;
    //     } else {
    //         l_s.reset();
    //         line_same = 999;
    //     }
    //     line_his = line_type;
    // }

    // void Defense::get_vector(void) {
    //     r_azimuth = gam.get_azimuth();
    //     get_line_value();
    //     if(line_detect) {
    //         if(line_azimuth < 45 && line_azimuth > 315) lastdetect = 0;
    //         else if(line_azimuth < 225 && line_azimuth > 135) lastdetect = 180;
    //         else lastdetect = line_azimuth;
    //     }
    //     myvector.get_cord(line_azimuth, line_dist);
    //     line_x = myvector.get_x();
    //     line_y = myvector.get_y();
    //     if(line_type != 2) {
    //         int line_absorate_azimuth = line_azimuth - r_azimuth;
    //         int la = line_absorate_azimuth % 360; if(la < 0) la += 360;
    //         line_sector = -1;
    //         if(la >= 30 && la <= 60) line_sector = 45;
    //         else if(la >= 120 && la <= 150) line_sector = 135;
    //         else if(la >= 210 && la <= 240) line_sector = 225;
    //         else if(la >= 300 && la <= 330) line_sector = 315;
    //         if(line_sector != -1) line_type = 2;
    //         else line_azimuth = line_sector;
    //     }
    //     int line_fb = 0;
    //     if(line_azimuth <= 90 || line_azimuth >= 270) line_fb = 1;
    //     else if(line_azimuth > 90 && line_azimuth < 270) line_fb = 2;
    //     line_go_ang = line_fb == 2 ? 180 : 0;
    //     line_power = 255 / (12 - line_dist);
    //     if(line_dist < 2) line_power = 0;
    //     ball.read();
    //     ball_azimuth = ball.get_azimuth();
    //     ball_dist = (int)ball.get_magnitude();
    //     ball_detect = ((int)ball.get_magnitude() > 0) ? true : false;
    //     myvector.get_cord(ball_azimuth, ball_dist);
    //     ball_x = myvector.get_x();
    //     ball_y = myvector.get_y();
    //     if(ball_detect) {
    //         ball_go_ang = (ball_x < 0) ? 270 : 90;
    //         ball_power = 150;
    //         if(ball_y < 120) ball_power = 180;
    //         if(ball_y < 70) ball_power = 200;
    //         ball_power = abs(ball_x) < catch_ball_X ? 0 : ball_power;
    //     } else {
    //         ball_power = 0;
    //         ball_x = 0;
    //         ball_go_ang = 0;
    //     }
    //     gotVector = 1;
    // }

    // void Defense::p1(void) {
    //     if(ball_power == 0) {
    //         p5();
    //     } else {
    //         move_x = ball_power * (ball_go_ang == 90 ? 1 : -1);
    //         move_y = line_power * (line_go_ang == 0 ? 1 : -1);
    //         move_power = myvector.get_magnitude(move_x * ball_rate, move_y * line_rate);
    //         move_azimuth = myvector.get_azimuth(move_x * ball_rate, move_y * line_rate);
    //         if(useMotor) mymotor.run(move_azimuth, move_power, 0);
    //     }
    // }

    // void Defense::p2(void) {
    //     mypixel.closest(line_azimuth, 0, 255, 0, 3);
    //     int m_rl = (ball_go_ang == 90 ? 1 : -1);
    //     int corner;
    //     if(line_azimuth > 0 && line_azimuth < 90) corner = 1;
    //     else if(line_azimuth > 90 && line_azimuth < 180) corner = 3;
    //     else if(line_azimuth > 180 && line_azimuth < 270) corner = 2;
    //     else if(line_azimuth > 270 && line_azimuth < 360) corner = 4;
    //     if(corner == 1) {
    //         if(ball_power != 0) {
    //             if(m_rl == 1) {
    //                 if(line_dist < p2_back_t) {
    //                     move_x = p2_back_s;
    //                     move_y = line_power * (line_go_ang == 0 ? 1 : -1);
    //                     int move_azimuth = myvector.get_azimuth(move_x, move_y * line_rate);
    //                     move_power = myvector.get_magnitude(move_x, move_y * line_rate);
    //                     if(useMotor) mymotor.run(move_azimuth, move_power * motor_rate, 0);
    //                 } else {
    //                     if(useMotor) mymotor.free();
    //                 }
    //             } else {
    //                 p1();
    //             }
    //         }
    //     }
    //     if(corner == 4) {
    //         if(ball_power != 0) {
    //             if(m_rl == -1) {
    //                 if(line_dist < p2_back_t) {
    //                     move_x = p2_back_s * -1;
    //                     move_y = line_power * (line_go_ang == 0 ? 1 : -1);
    //                     int move_azimuth = myvector.get_azimuth(move_x, move_y * line_rate);
    //                     move_power = myvector.get_magnitude(move_x, move_y * line_rate);
    //                     if(useMotor) mymotor.run(move_azimuth, move_power * motor_rate, 0);
    //                 } else {
    //                     if(useMotor) mymotor.free();
    //                 }
    //             } else {
    //                 p1();
    //             }
    //         }
    //     }
    //     if(corner == 2) {
    //         if(line_dist > p2_back_t) {
    //             mymotor.run(225, p2_back_s, 0);
    //         } else {
    //             if(ball_go_ang == 270) {
    //                 line_fb = 1;
    //                 p1();
    //             } else {
    //                 mymotor.free();
    //             }
    //         }
    //     }
    //     if(corner == 3) {
    //         if(line_dist > p2_back_t) {
    //             mymotor.run(135, p2_back_s, 0);
    //         } else {
    //             if(ball_go_ang == 90) {
    //                 line_fb = 1;
    //                 p1();
    //             } else {
    //                 mymotor.free();
    //             }
    //         }
    //     }
    // }

    // void Defense::p3(void) {
    //     mymotor.run(0, 100, 0);
    // }

    // inline void Defense::p3_move(int dir1, int dir2, int Power) {
    //     mymotor.run(0, 100, 0);
    // }

    // void Defense::p5(void) {
    //     if(line_detect) {
    //         if(line_dist > p5_t) {
    //             if(useMotor) mymotor.run(line_go_ang, line_power * motor_rate, 0);
    //         } else {
    //             if(!(r_azimuth < 15 && r_azimuth > 345)) {
    //                 if(useMotor) mymotor.run(0, 0, 0);
    //                 int d = 0 - r_azimuth;
    //                 if(d < 0) d += 360;
    //             } else {
    //                 if(useMotor) mymotor.free();
    //             }
    //         }
    //     } else {
    //         p4();
    //     }
    // }

    // void Defense::p4(void) {
    //     int backPower = 220;
    //     if(useMotor) mymotor.run(lastdetect, backPower, 0);
    // }

    // void Defense::debug1(void) {
    // }


//     #include "Defense.h"
// #include "Input.h"
// #include "Output.h"
// #include "AIP.h"
// #include "Process.h"

// //todo
// //ベクトルで主に動くように
// //シリアルコマンドも実装

// //powershell -ExecutionPolicy Bypass -File C:\Users\kusas\Desktop\program\auto-commit.ps1

// void Defense::timerReset(){
//     d_timer.reset();
//     silentTime.reset();
// }

// void Defense::setup(void){
//     // vectorの全要素を999で初期化
//     vector.ball_ang = 999;
//     vector.ball_power = 999;
//     vector.ball_x = 999;
//     vector.ball_y = 999;
//     vector.line_ang = 999;
//     vector.line_power = 999;
//     vector.line_x = 999;
//     vector.line_y = 999;
//     vector.go_ang = 999;
//     vector.move_power = 999;
//     vector.move_x = 999;
//     vector.move_y = 999;
//     IsDash= false;
//     d_timer.reset();
// }

// void Defense::defense_(void){
//     int Sread = silentTime.read_milli();
//     if(Sread>7000){
//         if(Sread>7000+dashTime){
//             IsDash=false;
//             silentTime.reset();
//         } else {
//             IsDash=true;
//         }
//     } else{
//         IsDash=false;
//     }
//     if(IsDash==false){//通常の動作
//         mybuzzer.stop();
//         resetUI();
//         get_value();
//         cal_vector();
//         move();
//         applyUI();
//     } else {//キーパーダッシュ時の動作
//         mybuzzer.start(100,999);
//         resetUI();
//         get_value();
//         dash();
//         applyUI();
//     }
// }

// void Defense::get_value(){
//     r_azimuth = gam.get_azimuth();
//     line_detect=line.get_magnitude()==999 ? false : true;
//     if(line_detect){
//         line_azimuth=norm360(line.get_azimuth());
//         line_absolute_angle=norm360(line_azimuth - r_azimuth);
//         line_dist=(line.get_magnitude()/100)+1;
//         lastdetect=line_azimuth;
//         myvector.get_cord(line_azimuth,line_dist);
//         line_x=myvector.get_x();
//         line_y=myvector.get_y();
//     } else {
//         line_azimuth=999;
//         line_absolute_angle=999;
//         line_dist=999;
//         line_x=999;
//         line_y=999;
//     }

//     ball_azimuth=ball.get_azimuth();
//     ball_absolute_angle=norm360(ball_azimuth - r_azimuth);
// }

// int Defense::cal_vector(){
//     int r=999;
//     //ボールのベクトルを算出
//     vector.ball_x = ball.get_x();
//     vector.ball_y = ball.get_y();
//     if(ball_azimuth<15||ball_azimuth>345){
//         vector.ball_ang=999;
//         vector.ball_power=0;
//     } else {
//         if(vector.ball_x<0){
//             vector.ball_ang=270;
//         } else {
//             vector.ball_ang=90;
//         }
//         vector.ball_power=150;
//     }

//     vector.line_x = line_x;
//     vector.line_y = line_y;
//     if (line_detect){
//         if(abs(line_x)>5){//角か縦　type分類も追加したいな
//             vector.line_ang=line_azimuth;
//             vector.line_power=line_dist*40;
//         } else {//これは直線ライン　角度は前後ろのみ
//             vector.line_ang=line_y<0?180:0;
//             vector.line_power=line_dist*20;
//         }
//         mybuzzer.stop();
//     } else {
//         mybuzzer.start(1000,999);
//         vector.go_ang=-lastdetect;
//         vector.move_power=200;
//         return r;
//     }

//     if(vector.ball_power<30||ball.get_stat()==0){//ボールが目の前にあるときは停止
//         vector.move_power=vector.line_power;
//         vector.go_ang=vector.line_ang;

//     } else if(diff_signs(ball_x,line_x)&&abs(line_x)>5){//lineが角か縦のときはlineの方向を”優先”
//         vector.move_power=vector.line_power;
//         vector.go_ang=vector.line_ang;
//     } else {//縦ライン時の通常動作　xはボール　yはライン　ラインのx座標適用は角か縦のみ

//         vector.move_y=(vector.line_y*line_late);
//         if(vector.move_y>line_max)vector.move_y=line_max;//ラインのパワーを強くしすぎないように最大を設定

//         vector.move_x=vector.ball_x;

//         double angle_rad = atan2(vector.move_y, vector.move_x); // y, x の順
//         double angle_deg = angle_rad * 180.0 / M_PI;
//         angle_deg = norm360(angle_deg + 90);
//         vector.go_ang = angle_deg;
//         vector.move_power=myvector.get_magnitude(vector.move_x,vector.move_y);
//     }
//     return r;
// }

// void Defense::move(){
//         last_power=vector.move_power;
//         if(vector.move_power<30){
//             mymotor.free();
//             uiMode=1;
//         } else {
//             silentTime.reset();
//             mymotor.run(vector.go_ang,vector.move_power,0);
//         }
//         mypixel.closest(vector.line_ang,0,255,0,1);
// }

// void Defense::resetUI(){
//     uiMode=0;
//     //背景 #39c5bccc
//     background.red = 57;
//     background.green = 197;
//     background.blue = 187;
//     background.alpha = 0.8;
//     //ボール #ff00007f
//     P_ball.red = 255;
//     P_ball.green = 0;
//     P_ball.blue = 0;
//     P_ball.alpha = 0.5;
//     //ライン #00b4787f
//     P_line.red = 0;
//     P_line.green = 180;
//     P_line.blue = 120;
//     P_line.alpha = 0.5;
//     //前方の角度　#FF00647F
//     P_r_azimuth.red = 255;
//     P_r_azimuth.green = 0;
//     P_r_azimuth.blue = 100;
//     P_r_azimuth.alpha = 0.5;
//     //進行方向 #00FF00FF
//     go_ang.red = 0;
//     go_ang.green = 255;
//     go_ang.blue = 0;
//     go_ang.alpha = 1;
// }

// void Defense::applyUI(){
//     //alphaの適用　　　
//         background.red = background.red*background.alpha;
//         background.green = background.green*background.alpha;
//         background.blue = background.blue*background.alpha;
//     P_ball.red = P_ball.red*P_ball.alpha;
//     P_ball.green = P_ball.green*P_ball.alpha;
//     P_ball.blue = P_ball.blue*P_ball.alpha;
//         P_line.red = P_line.red*P_line.alpha;
//         P_line.green = P_line.green*P_line.alpha;
//         P_line.blue = P_line.blue*P_line.alpha;
//     P_r_azimuth.red = P_r_azimuth.red*P_r_azimuth.alpha;
//     P_r_azimuth.green = P_r_azimuth.green*P_r_azimuth.alpha;
//     P_r_azimuth.blue = P_r_azimuth.blue*P_r_azimuth.alpha;
//         go_ang.red = go_ang.red*go_ang.alpha;
//         go_ang.green = go_ang.green*go_ang.alpha;
//         go_ang.blue = go_ang.blue*go_ang.alpha;

//     if(uiMode==0){
//     mypixel.multi(0, 15, background.red, background.green, background.blue);
//     mypixel.closest(line_azimuth,P_line.red,P_line.green,P_line.blue,1);
//     mypixel.closest(ball_azimuth,P_ball.red,P_ball.green,P_ball.blue,1);
//     mypixel.closest(r_azimuth,P_r_azimuth.red,P_r_azimuth.green,P_r_azimuth.blue,1);
//     mypixel.closest(vector.go_ang,go_ang.red,go_ang.green,go_ang.blue,1);
//     } else if(uiMode==1){
//         // 7000msのうち何％か
//         float percent = (float)silentTime.read_milli() / 7000.0;
//         if(percent > 1.0) percent = 1.0;
//         // 16ピクセル中何個光らせるか
//         int num_pixels = (int)(percent * 16);
//         mypixel.multi(0, 15, background.red, background.green, background.blue);
//         if(num_pixels > 0){
//             mypixel.multi(0, num_pixels-1, 255, 0, 255); // 0～num_pixels-1まで点灯
//         }
//     }
// }

// void Defense::ReadCommand(){
// };

// void Defense::dash(){
//     attack.attack_();
// };

// bool Defense::useSerial(bool use) {
//     SerialEnabled = use;
//     return SerialEnabled;
// }

};
