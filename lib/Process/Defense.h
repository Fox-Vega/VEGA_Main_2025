#pragma once
#include <Arduino.h>
#include <Timer.h>

class Defense {
    public:
        void setup(void);                  // 初期設定
        void defense_(void);               // ディフェンス処理
    private:
        // --- 調整用定数 ---
        const int ball_max_X = 300;        // ボールの最大x座標（処理用）
        const int catch_ball_X = 30;       // ボールが目の前にあると判定するライン
        const int line_rate = 1;           // ラインの倍率
        const int ball_rate = 1.2;         // ボールの倍率
        const int motor_rate = 1;          // モーターの倍率

        // --- ボール関連変数 ---
        int ball_azimuth;                  // ボールの方位角
        int ball_dist;                     // ボールの距離
        float ball_x;                      // ボールのx座標
        float ball_y;                      // ボールのy座標
        bool ball_detect;                  // ボールが検出されているかどうか
        int ball_power = 0;               // ボールのパワー
        int ball_go_ang = 0;              // ボールの進行方向

        // --- ライン関連変数 ---
        int line_azimuth;                  // ラインの方位角
        int line_dist;                     // ラインの距離
        float line_x;                      // ラインのx座標
        float line_y;                      // ラインのy座標
        bool line_detect;                  // ラインが検出されているかどうか
        short line_type;                   // ラインの種類
        int line_go_ang;                   // ラインの進行方向
        int line_fb;                       // ラインの前後
        int line_power;                    // ラインのパワー

        // --- その他処理用変数 ---
        int r_azimuth;                     // ロボットの方位角
        int go_ang;                        // 進行方向
        int move_x;                        // 動くx
        int move_y;                        // 動くy
        int move_power;                    // 動くパワー
        int lastdetect = 180;              // 最後の検出角度 デフォルト後ろ
        int gotVector = 0;                 // ベクトル取得フラグ　多重取得防止やけど使ってない
        Timer d_timer;

        // --- メソッド ---
        void get_vector(void);             // ベクトルを取得する関数
        void p1(void);                     // 水平ライン
        void p2(void);                     // コーナー　　まだ
        void p3(void);                     // 垂直ライン　まだ
        void p4(void);                     // ラインに戻る　不安定
        void p5(void);                     // ラインを追跡　まあまあ
        void keeper_dash(void);            // キーパーのダッシュ　まだ
        void debug1(void);                 // デバッグ用1　めっちゃ重い
};