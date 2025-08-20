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
        // --- その他処理用変数 ---
        int r_azimuth;
        Timer d_timer;

        // --- メソッド ---
        void get_value();
        void cal_vector();
        void move();
        void dash();
        void back();
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
};