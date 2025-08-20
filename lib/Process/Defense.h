#pragma once
#include <Arduino.h>
#include <Timer.h>

//ちなpはプロセスの略

class Defense {
    public:
        void setup(void);                  // 初期設定
        void defense_(void);               // メイン処置
        bool useSerial(bool use);           // シリアルを使うかどうか　コマンドで変える
    private:
        // モード定義
        bool SerialEnabled = false;       // シリアル通信の有効/無効
        bool PixelEnabled = true;
        // --- 調整用定数 ---

        // --- ボール関連変数 ---

        bool ball_detect;// ボールが検出されているかどうか
        int ball_azimuth;// ボールの方位角
        int ball_absolute_angle;// ボールの絶対角度
        int ball_dist;// ボールの距離
        int ball_x;// ボールのx座標
        int ball_y;// ボールのy座標

        // --- ライン関連変数 ---
        bool line_detect;// ラインが検出されているかどうか
        int line_azimuth;// ラインの方位角
        int line_absolute_angle;// ラインの絶対角度
        int line_dist;// ラインの距離
        int lastdetect;// 最後に検出したラインの方位角
        int line_x;// ラインのx座標
        int line_y;// ラインのy座標

        //　--- ベクトル ---
        struct vector {//   ベクトル
            int ball_ang;// ボールの動きたい方向
            int ball_power;// ボールのパワー
            int ball_x;// ボールのx座標
            int ball_y;// ボールのy座標

            int line_ang;// ラインの動きたい方向
            int line_power;// ラインのパワー
            int line_x;// ラインのx座標
            int line_y;// ラインのy座標

            int go_ang;//動く方向
            int move_power;//動く強さ
            int move_x;//動くx
            int move_y;//動くy
        };
        vector vector;

        // ---NeoPixel---
        struct RGBA {
            int red;
            int green;
            int blue;
            float alpha;
        };

        RGBA background;
        RGBA P_line;
        RGBA P_ball;
        RGBA P_r_azimuth;
        RGBA go_ang;

        // --- その他処理用変数 ---
        int mode;
        int r_azimuth;
        Timer d_timer;

        // --- メソッド ---
        void get_value();
        int cal_vector();
        void move();
        void dash();
        void back();
        void ReadCommand();
        void resetUI();
        void applyUI();
        inline int norm360(int a){a%=360;if(a < 0)a+=360;return a;};



        // --- 調整用定数 ---

        // //倍率
        // const float line_rate = 1;           // ラインの倍率
        // const float ball_rate = 1;         // ボールの倍率
        // const float motor_rate = 1;          // モーターの倍率

        // //距離
        // const uint16_t ball_max_X = 300;        // ボールの最大x座標（処理用）
        // const uint8_t catch_ball_X = 10;       // ボールが目の前にあると判定するライン

        // const uint8_t p5_t = 5;                   // p5のトリガー
        // const uint8_t p2_back_t=3;               //　p2の戻る距離トリガー

        // //速度
        // const uint8_t p2_back_s=100;               // p2の戻る速度

        // //タイマー
        // const uint16_t p3_move_t = 3200; // 垂直ラインの移動処理用タイマー

        // // --- ボール関連変数 ---
        // int ball_azimuth;                  // ボールの方位角
        // int ball_absolute_angle;           // 機体角度を考慮したボールの絶対角度
        // int ball_dist;                     // ボールの距離
        // float ball_x;                      // ボールのx座標
        // float ball_y;                      // ボールのy座標
        // bool ball_detect;                  // ボールが検出されているかどうか
        // uint8_t ball_power = 0;               // ボールのパワー
        // int ball_go_ang = 0;              // ボールの進行方向

        // // --- ライン関連変数 ---
        // int line_azimuth;                  // ラインの方位角
        // uint8_t line_dist;                     // ラインの距離
        // float line_x;                      // ラインのx座標
        // float line_y;                      // ラインのy座標
        // bool line_detect;                  // ラインが検出されているかどうか
        // uint8_t line_type;                   // ラインの種類
        // int line_go_ang;                   // ラインの進行方向
        // uint8_t line_fb;                       // ラインの前後
        // uint8_t line_power;                    // ラインのパワー

        // // --- その他処理用変数 ---
        // bool usePixel = true;              // ピクセルを使用するかどうか
        // bool useBall = true;                // ボールを使用するかどうか
        // bool useMotor = true;               // モーターを使用するかどうか
        // bool useTimer = false;               // タイマーを使用するかどうか
        // int line_sector;
        // int r_azimuth;                     // ロボットの方位角
        // int go_ang;                        // 進行方向
        // int move_x;                        // 動くx
        // int move_y;                        // 動くy
        // uint8_t move_power;                // 動くパワー
        // int move_azimuth;                  // 動く方位角
        // int lastdetect = 180;              // 最後の検出角度 デフォルト後ろ
        // int gotVector = 0;                 // ベクトル取得フラグ　多重取得防止やけど使ってない
        // Timer d_timer;                     //デバッグ用
        // Timer l_s;                         //line_same
        // Timer p3_timer;                    // 垂直ラインの移動処理用タイマー
        // int line_same=999;
        // int line_his;

        // // --- メソッド ---
        // void get_vector(void);             // ベクトルを取得する関数
        // void p1(void);                     // 水平ライン
        // void p2(void);                     // コーナー　　まだ
        // void p3(void);                     // 垂直ライン　まだ
        // void p4(void);                     // ラインに戻る　不安定
        // void p5(void);                     // ラインを追跡　まあまあ
        // inline void get_line_value(void);  //　inline値取得
        // inline void p3_move(int dir1,int dir2, int Power); // 垂直ラインの移動処理
        // void keeper_dash(void);            // キーパーのダッシュ　まだ
        // void debug1(void);                 // デバッグ用1　めっちゃ重い

        //前ディフェンス

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


};