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

    // === ボールフィルタ ===
    void ball_filter();                // ボール角度フィルタリング
    int ball_get_Aazimuth();          // フィルタ済み角度取得

private:
    // === 調整用定数 ===
    #define BALL_FILTER_OFF   // ボールフィルタを無効にする場合
    #define USE_DASH true
    static constexpr float dash_border = 5000.0;        // ダッシュ待ち時間
    static constexpr float dash_time = 1500.0;          // ダッシュ時間
    static constexpr float ball_move_border = 15.0;      // ボール移動境界(±角度)
    static constexpr float ball_power = 200.0;          // ボール対応パワー(x軸)
    static constexpr float move_border = 50.0;          // 移動最小値
    static constexpr float ballFilter = 7.0;           // ボールフィルタ閾値(度)
    static constexpr float exitCorner = 30.0;           // コーナー退避距離
    static constexpr float line_late = 1.0;         // ライン反応倍率
    static constexpr float ball_late = 1.0;         // ボール反応倍率
    static constexpr float line_max = 100.0;        // ライン移動最大値

    // === 処理用変数 ===
    int frog;                          //フラグ　1ノーマル 2ラインなし 3ボールなし 4角 5角(賭け) 6移動量
    int lastdetect;                    // 最後検出方向
    int move_azimuth;                  // 移動方向
    float move_power;                    // 移動パワー
    double move_x;                     // X軸移動量
    double move_y;                     // Y軸移動量
    bool ball_move = false;            // ボール移動フラグ
    float line_r = 0;                  // ライン半径
    bool ex = false;                   // 追加移動フラグ
    int exmoveX;                       // 追加移動X角度
    int exmoveY;                       // 追加移動Y角度
    Timer Dtimer;                      // ディフェンスタイマー
    Timer SilentTime;

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

    //誤差測定
    inline int getErr(int a, int b) {
        return (a - b + 540) % 360 - 180;
    }

    // === UI用構造体 ===
    struct RGBA {
        int red;
        int green;
        int blue;
        float alpha;
    };

    // === UI用変数 ===
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
