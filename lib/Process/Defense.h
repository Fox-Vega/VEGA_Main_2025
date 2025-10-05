#pragma once
#include <Arduino.h>
#include <Timer.h>

// Defense - ディフェンス処理クラス

class Defense {
public:
    // === メイン処理 ===
    void setup(void);                  // 初期設定
    void defense_(void);               // メイン処理
    void reset();                      // リセット処理

private:
    // === 調整用定数 ===
    //#define BALL_FILTER_OFF   // ボールフィルタを無効にする場合
    #define USE_DASH false
    static constexpr float dash_border = 5000.0;        // ダッシュ待ち時間
    static constexpr float dash_time = 1500.0;          // ダッシュ時間
    static constexpr float ball_move_border = 7.0;      // ボール移動境界(±角度)
    static constexpr float ball_power = 220.0;          // ボール対応パワー(x軸)
    static constexpr float move_border = -10.0;          // 移動最小値
    static constexpr float line_late = 0.8;         // ライン反応倍率
    static constexpr float ball_late = 1.0;         // ボール反応倍率
    

    // === 処理用変数 ===
    int frog;                          //フラグ　1ノーマル 2ラインなし 3ボールなし 4角 5角(賭け) 6移動量
    int lastdetect;                    // 最後検出方向
    int move_azimuth;                  // 移動方向
    float move_power;                  // 移動パワー
    double move_x;                     // X軸移動量
    double move_y;                     // Y軸移動量

    float rad;
    float ball_ang;                    // ボール回避方向
    float line_ang;
    float line_x;
    float line_y;
    float ball_x;
    float ball_y;
    float line_r = 0;                  // ライン半径
    Timer Dtimer;                      // ディフェンスタイマー
    Timer SilentTime;
    bool timer_started;


    int ppp;
    // === ボールフィルタ用 ===
    int ball_history[5] = {0,0,0,0,0}; // ボール角度履歴

    // === ユーティリティ関数 ===

    //360補正
    inline int norm360(int a) {
        a %= 360;
        if(a < 0) a += 360;
        return a;
    }

    //符号違い判定
    inline bool diff_signs(int a, int b) {
        return (a >= 0 && b < 0) || (a < 0 && b >= 0);
    }

    //誤差測定（度→ラジアンで最短差分を計算し、度で返す）
    inline int getErr(int a, int b) {
        float dRad = atan2f(sinf((float)(b - a) * DEG_TO_RAD), cosf((float)(b - a) * DEG_TO_RAD));
        int dDeg = (int)(dRad * RAD_TO_DEG); // -180～180 の範囲
        return dDeg;
    }

    // === UI用構造体 ===
    struct RGBA {
        int red;
        int green;
        int blue;
        float alpha;
        void reset(){
            red=0;
            green=0;
            blue=0;
            alpha=0;
        }
        void applyAlpha(){
            red=red*alpha;
            green=green*alpha;
            blue=blue*alpha;
            alpha=1.0;
        }
    };

    // === UI0用変数 ===
    RGBA background;                   // 背景色
    RGBA P_line;                       // ライン表示色
    RGBA P_ball;                       // ボール表示色
    RGBA move_ang;                     // 移動方向表示色
    RGBA exMoveX;                      // 追加移動X軸色
    RGBA exMoveY;                      // 追加移動Y軸色
    RGBA dash_timer;                   // ダッシュタイマー色

    // === UI処理 ===

    // UI色設定
    void resetUI();
    // UI表示実行
    void applyUI(int mode);
};
