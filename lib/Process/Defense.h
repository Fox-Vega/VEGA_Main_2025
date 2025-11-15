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
    #define USE_DASH true
    static constexpr int ball_cal =-10;
    static constexpr float dash_border = 5000.0;        // ダッシュ待ち時間
    static constexpr float dash_time = 1200.0;          // ダッシュ時間
    static constexpr int dash_border_ball = 2000;    // ボール検出でダッシュ待ち時間リセット
    static constexpr float ball_move_border = 7.0;      // ボール移動境界(±角度)
    static constexpr float move_speed = 300.0;          // 移動スピード（旧: ball_power）
    static constexpr float move_border = 50.0;          // 移動最小値 -o判定に使う
    static constexpr float line_late = 1.0;         // ライン反応倍率
    static constexpr float y_late = 1.0;         // ライン反応倍率
    static constexpr float x_late = 1.0;
    static constexpr float ball_late = 1.0;         // ボール反応倍率
    static constexpr float calblate_sir_late = 1.2;

    static constexpr uint_fast8_t edge_list[6] = {5,6,7,17,18,19};

    // === 処理用変数 ===
    static int lastdetect[2];                    // 最後検出方向
    static int move_azimuth;                  // 移動方向
    static float move_power;                  // 移動パワー
    static double move_x;                     // X軸移動量
    static double move_y;                     // Y軸移動量
    static int calc_move_speed;             // 速度減算
    static int lastpower;
    static int last_x;
    static int last_y;

    static float rad;
    static float ball_ang;                    // ボール回避方向
    static float line_x;
    static float line_y;
    static float ball_x;
    static float ball_y;
    static Timer Dtimer;                      // ディフェンスタイマー
    static Timer SilentTime;

    static int calb;
    static bool tl;
    static bool edge;

    // === ユーティリティ関数 ===

    //360補正


    /// @brief 360度の範囲に正規化する
    /// @param a 正規化する角度　単位：度
    /// @return 正規化された角度
    inline int norm360(int a) {
        a %= 360;
        if(a < 0) a += 360;
        return a;
    }

    //誤差測定　差分の絶対値を返す
    inline int getErr(int a, int b) { int d = abs((a - b) % 360); return (d > 180) ? (360 - d) : d; }

    inline bool diff_signs(int a, int b) {
        return (a >= 0 && b < 0) || (a < 0 && b >= 0);
    }

    /// @brief 角度を0, 90, 180, 270の最も近い値に丸める
    /// @param angle 入力角度（0-359）
    /// @return 最も近い基本方向（0, 90, 180, 270）
    inline int roundToCardinal(int angle) {
        angle = norm360(angle);  // 0-359に正規化
        
        // 各方向との誤差を計算
        int err0 = getErr(angle, 0);
        int err90 = getErr(angle, 90);
        int err180 = getErr(angle, 180);
        int err270 = getErr(angle, 270);
        
        // 最小誤差の方向を返す
        int minErr = err0;
        int result = 0;
        
        if (err90 < minErr) {
            minErr = err90;
            result = 90;
        }
        if (err180 < minErr) {
            minErr = err180;
            result = 180;
        }
        if (err270 < minErr) {
            minErr = err270;
            result = 270;
        }
        
        return result;
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

    // === UI用変数 ===
    static RGBA background;                   // 背景色
    static RGBA P_line;                       // ライン表示色
    static RGBA P_ball;                       // ボール表示色
    static RGBA move_ang;                     // 移動方向表示色
    static RGBA dash_timer;                   // ダッシュタイマー色

    enum class FROG : int {
        NONE = 0,
        NORMAL = 1,     // ノーマル
        NO_LINE = 2,    // ラインなし
        NO_BALL = 3,    // ボールなし
        STOP = 4, // 停止
        DASH = 5      // ダッシュ中
    };
    FROG frog;                          //フラグ　1ノーマル 2ラインなし 3ボールなし 4停止　5ダッシュ中

    // === UI処理 ===

    // UI色設定
    void resetUI();
    // UI表示実行
    void applyUI(int mode);

};
