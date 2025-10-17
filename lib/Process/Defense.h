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
    static constexpr float dash_border = 5000.0;        // ダッシュ待ち時間
    static constexpr float dash_time = 1500.0;          // ダッシュ時間
    static constexpr float ball_move_border = 5.0;      // ボール移動境界(±角度)
    static constexpr float move_speed = 220.0;          // 移動スピード（旧: ball_power）
    static constexpr float move_border = 40.0;          // 移動最小値 判定に使う
    static constexpr float line_late = 1.0;         // ライン反応倍率
    static constexpr float y_late = 0.9;         // ライン反応倍率
    static constexpr float x_late = 1.0;
    static constexpr float ball_late = 1.0;         // ボール反応倍率

    // === 処理用変数 ===
    int lastdetect;                    // 最後検出方向
    int move_azimuth;                  // 移動方向
    float move_power;                  // 移動パワー
    double move_x;                     // X軸移動量
    double move_y;                     // Y軸移動量

    float rad;
    float ball_ang;                    // ボール回避方向
    float line_x;
    float line_y;
    float ball_x;
    float ball_y;
    Timer Dtimer;                      // ディフェンスタイマー
    Timer SilentTime;

    // === ユーティリティ関数 ===

    //360補正


    /// @brief 360度の範囲に正規化する
    /// @param a 正規化する角度　単位：度
    /// @return 正規化された角度
    /// @details a を 360 で剰余した後、負値は 360 を足して 0..359 にします。
    /// @code
    /// int x = norm360(-90); // -> 270
    /// int y = norm360(370); // -> 10
    /// @endcode
    inline int norm360(int a) {
        a %= 360;
        if(a < 0) a += 360;
        return a;
    }

    //誤差測定　差分の絶対値を返す
    inline int getErr(int a, int b) { int d = abs((a - b) % 360); return (d > 180) ? (360 - d) : d; }

    //姿勢制御のせいで動かん
    int m_ang[8] = {0,45,90,135,180,225,270,315};
    //m_angの中で一番近い角度を返す
    inline int getClosestAngle(int target) {
        int minErr = 360;
        int closest = m_ang[0];

        for(int i = 0; i < 8; i++) {
            int err = getErr(target, m_ang[i]);
            if(err < minErr) {
                minErr = err;
                closest = m_ang[i];
            }
        }
        return closest;
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
    RGBA dash_timer;                   // ダッシュタイマー色

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
