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
    static int ddddd; //デバッグ用

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
    static bool frog1;
    static bool frog2;

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


//             {
    //             // 計算式表示用の変数準備
    //             double line_component_x = line_x * line_late * x_late * calc_move_speed;
    //             double ball_component_x = ball_x * ball_late * x_late * calc_move_speed;
    //             double line_component_y = line_y * line_late * y_late * calc_move_speed;
    //             double ball_component_y = ball_y * ball_late * y_late * calc_move_speed;
                
    //             printf_s_long(R"RAW(
    // =================================================================================================================
    // === センサー入力値 ===
    // line.get_azimuth()    : %d度
    // line.get_type()       : %d
    // line.get_x()          : %d
    // line.get_y()          : %d
    // line.get_magnitude()  : %d
    // ball.get_azimuth()    : %d度
    // ball.get_stat()       : %d
    // gam.get_azimuth()     : %d度
    
    // === フラグ ===
    // tl (縦ライン)         : %d
    // edge (エッジ)         : %d
    
    // === 定数パラメータ ===
    // ball_cal              : %d
    // move_speed            : %d.%02d
    // calc_move_speed       : %d
    // line_late             : %d.%02d
    // x_late                : %d.%02d
    // y_late                : %d.%02d
    // ball_late             : %d.%02d
    // ball_move_border      : %d.%02d
    // move_border           : %d.%02d
    
    // === 角度計算 ===
    // calb = 0 - gam.get_azimuth()
    //      = 0 - %d = %d
    
    // ball_ang = ball.get_azimuth() + ball_cal
    //          = %d + %d = %d.%02d度
    
    // rad = radians(line.get_azimuth())
    //     = radians(%d) = %d.%04d rad
    
    // === ライン方向ベクトル ===
    // line_x = sin(rad) = sin(%d.%04d)
    //        = %d.%04d
    // line_y = cos(rad) = cos(%d.%04d)
    //        = %d.%04d
    
    // === ボール方向判定 ===
    // ball_y = (ball_ang<90 || ball_ang>270) ? 1 : -1
    //        = (%d.%02d<90 || %d.%02d>270) ? 1 : -1
    //        = %d
    // ball_x = (ball_ang<180) ? 1 : -1
    //        = (%d.%02d<180) ? 1 : -1
    //        = %d
    
    // === 速度減算計算 ===
    // calc_move_speed = (line.get_x()>3 || tl) ? move_speed>>1 : move_speed
    //                 = (%d>3 || %d) ? %d>>1 : %d
    //                 = %d
    
    // === X軸移動量計算 ===
    // line_component_x = line_x * line_late * x_late * calc_move_speed
    //                  = %d.%04d * %d.%02d * %d.%02d * %d
    //                  = %d.%02d
    // ball_component_x = ball_x * ball_late * x_late * calc_move_speed
    //                  = %d * %d.%02d * %d.%02d * %d
    //                  = %d.%02d
    // move_x = line_component_x + ball_component_x
    //        = %d.%02d + %d.%02d
    //        = %d.%02d
    // [tl補正後] move_x = %d.%02d
    
    // === Y軸移動量計算 ===
    // line_component_y = line_y * line_late * y_late * calc_move_speed
    //                  = %d.%04d * %d.%02d * %d.%02d * %d
    //                  = %d.%02d
    // ball_component_y = ball_y * ball_late * y_late * calc_move_speed
    //                  = %d * %d.%02d * %d.%02d * %d
    //                  = %d.%02d
    // move_y = line_component_y + ball_component_y
    //        = %d.%02d + %d.%02d
    //        = %d.%02d
    // [補正後] move_y = %d.%02d
    
    // === 最終移動ベクトル ===
    // move_azimuth = myvector.get_azimuth(move_x, move_y)
    //              = myvector.get_azimuth(%d.%02d, %d.%02d)
    //              = %d度
    // move_power = myvector.get_magnitude(abs(move_x), abs(move_y))
    //            = myvector.get_magnitude(%d.%02d, %d.%02d)
    //            = %d.%02d
    
    // === 停止判定 ===
    // getErr(0+ball_cal, ball.get_azimuth()) < ball_move_border && !tl
    // = getErr(%d, %d) < %d.%02d && !%d
    // = %d < %d.%02d && !%d
    // = %d (停止判定: %s)
    
    // === モーター制御 ===
    // move_power > move_border: %d.%02d > %d.%02d = %s
    // 実行: %s
    
    // ========================================
    // )RAW",
    //             // センサー入力値
    //             line.get_azimuth(), line.get_type(),
    //             static_cast<int>(line.get_x()), static_cast<int>(line.get_y()),
    //             static_cast<int>(line.get_magnitude()),
    //             ball.get_azimuth(), ball.get_stat(),
    //             gam.get_azimuth(),
                
    //             // フラグ
    //             tl, edge,
                
    //             // 定数パラメータ
    //             ball_cal,
    //             static_cast<int>(move_speed), static_cast<int>(move_speed * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(line_late), static_cast<int>(line_late * 100) % 100,
    //             static_cast<int>(x_late), static_cast<int>(x_late * 100) % 100,
    //             static_cast<int>(y_late), static_cast<int>(y_late * 100) % 100,
    //             static_cast<int>(ball_late), static_cast<int>(ball_late * 100) % 100,
    //             static_cast<int>(ball_move_border), static_cast<int>(ball_move_border * 100) % 100,
    //             static_cast<int>(move_border), static_cast<int>(move_border * 100) % 100,
                
    //             // 角度計算
    //             gam.get_azimuth(), calb,
    //             ball.get_azimuth(), ball_cal,
    //             static_cast<int>(ball_ang), static_cast<int>(ball_ang * 100) % 100,
    //             line.get_azimuth(),
    //             static_cast<int>(rad), static_cast<int>(rad * 10000) % 10000,
                
    //             // ライン方向ベクトル
    //             static_cast<int>(rad), static_cast<int>(rad * 10000) % 10000,
    //             static_cast<int>(line_x), static_cast<int>(abs(line_x) * 10000) % 10000,
    //             static_cast<int>(rad), static_cast<int>(rad * 10000) % 10000,
    //             static_cast<int>(line_y), static_cast<int>(abs(line_y) * 10000) % 10000,
                
    //             // ボール方向判定
    //             static_cast<int>(ball_ang), static_cast<int>(ball_ang * 100) % 100,
    //             static_cast<int>(ball_ang), static_cast<int>(ball_ang * 100) % 100,
    //             static_cast<int>(ball_y),
    //             static_cast<int>(ball_ang), static_cast<int>(ball_ang * 100) % 100,
    //             static_cast<int>(ball_x),
                
    //             // 速度減算計算
    //             static_cast<int>(line.get_x()), tl,
    //             static_cast<int>(move_speed), static_cast<int>(move_speed),
    //             calc_move_speed,
                
    //             // X軸移動量計算
    //             static_cast<int>(line_x), static_cast<int>(abs(line_x) * 10000) % 10000,
    //             static_cast<int>(line_late), static_cast<int>(line_late * 100) % 100,
    //             static_cast<int>(x_late), static_cast<int>(x_late * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(line_component_x), static_cast<int>(abs(line_component_x) * 100) % 100,
    //             static_cast<int>(ball_x),
    //             static_cast<int>(ball_late), static_cast<int>(ball_late * 100) % 100,
    //             static_cast<int>(x_late), static_cast<int>(x_late * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(ball_component_x), static_cast<int>(abs(ball_component_x) * 100) % 100,
    //             static_cast<int>(line_component_x), static_cast<int>(abs(line_component_x) * 100) % 100,
    //             static_cast<int>(ball_component_x), static_cast<int>(abs(ball_component_x) * 100) % 100,
    //             static_cast<int>(line_component_x + ball_component_x), static_cast<int>(abs(line_component_x + ball_component_x) * 100) % 100,
    //             static_cast<int>(move_x), static_cast<int>(abs(move_x) * 100) % 100,
                
    //             // Y軸移動量計算
    //             static_cast<int>(line_y), static_cast<int>(abs(line_y) * 10000) % 10000,
    //             static_cast<int>(line_late), static_cast<int>(line_late * 100) % 100,
    //             static_cast<int>(y_late), static_cast<int>(y_late * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(line_component_y), static_cast<int>(abs(line_component_y) * 100) % 100,
    //             static_cast<int>(ball_y),
    //             static_cast<int>(ball_late), static_cast<int>(ball_late * 100) % 100,
    //             static_cast<int>(y_late), static_cast<int>(y_late * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(ball_component_y), static_cast<int>(abs(ball_component_y) * 100) % 100,
    //             static_cast<int>(line_component_y), static_cast<int>(abs(line_component_y) * 100) % 100,
    //             static_cast<int>(ball_component_y), static_cast<int>(abs(ball_component_y) * 100) % 100,
    //             static_cast<int>(line_component_y + ball_component_y), static_cast<int>(abs(line_component_y + ball_component_y) * 100) % 100,
    //             static_cast<int>(move_y), static_cast<int>(abs(move_y) * 100) % 100,
                
    //             // 最終移動ベクトル
    //             static_cast<int>(move_x), static_cast<int>(abs(move_x) * 100) % 100,
    //             static_cast<int>(move_y), static_cast<int>(abs(move_y) * 100) % 100,
    //             move_azimuth,
    //             static_cast<int>(abs(move_x)), static_cast<int>(abs(move_x) * 100) % 100,
    //             static_cast<int>(abs(move_y)), static_cast<int>(abs(move_y) * 100) % 100,
    //             static_cast<int>(move_power), static_cast<int>(move_power * 100) % 100,
                
    //             // 停止判定
    //             0 + ball_cal, ball.get_azimuth(),
    //             static_cast<int>(ball_move_border), static_cast<int>(ball_move_border * 100) % 100,
    //             tl,
    //             getErr(0 + ball_cal, ball.get_azimuth()),
    //             static_cast<int>(ball_move_border), static_cast<int>(ball_move_border * 100) % 100,
    //             tl,
    //             (getErr(0 + ball_cal, ball.get_azimuth()) < ball_move_border && !tl),
    //             (getErr(0 + ball_cal, ball.get_azimuth()) < ball_move_border && !tl) ? "停止" : "動作",
                
    //             // モーター制御
    //             static_cast<int>(move_power), static_cast<int>(move_power * 100) % 100,
    //             static_cast<int>(move_border), static_cast<int>(move_border * 100) % 100,
    //             (move_power > move_border) ? "true" : "false",
    //             (move_power > move_border) ? "mymotor.run()" : "mymotor.free()"
    //             );
    //             delay(100);  // シリアル出力の安定化のため待機
    //             }
    