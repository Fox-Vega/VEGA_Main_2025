#pragma once
#include <Arduino.h>
#include <Timer.h>

// Defense - ディフェンス処理クラス

class Defense {
public:
    // === メイン処理 ===
    /// @brief Defenseクラスの初期設定を行う
    void setup(void);
    /// @brief Defenseのメイン処理ループ
    void defense_(void);
    /// @brief Defenseの状態をリセットする
    void reset();

private:

// === 調整用定数 ===
//#define BALL_FILTER_OFF   // ボールフィルタを無効にする場合
#define USE_DASH true

static constexpr int ball_cal =-10;
static constexpr float dash_border = 13000.0;        // ダッシュ待ち時間
static constexpr float dash_time = 15000.0;          // ダッシュ時間
static constexpr int dash_border_ball = 2000;    // ボール検出でダッシュ待ち時間リセット
static constexpr int vertical_exit=1500;
static constexpr int vertical_return=250;
static constexpr float ball_move_border = 7.0;      // ボール移動境界(±角度)





static constexpr float move_speed = 200.0;          // 移動スピード（旧: ball_power）
static constexpr float move_border = 50.0;          // 移動最小値 -o判定に使う
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
static Timer MoveTime;
static Timer ReturnTime;
static Timer verticalTime;
    static int calb;
    static bool tl;
    static bool edge;
    static bool corner;
    static bool frog1;
    static bool frog2;

    // === 入力キャッシュ変数 ===
    static int line_azimuth_cache;
    static int line_type_cache;
    static int line_x_cache;
    static int line_y_cache;
    static int ball_azimuth_cache;
    static bool ball_stat_cache;
    static int gam_azimuth_cache;

    // === ヘルパーメソッド ===
    void dash();
    void normal();
    void noline();
    void noball();
    void updateTimers();


    // === ユーティリティ関数 ===

    /// @brief 360度の範囲に正規化する
    /// @param a 正規化する角度　単位：度
    /// @return 正規化された角度
    inline static int norm360( int a) {
        a %= 360;
        if(a < 0) a += 360;
        return a;
    }

    /// @brief 360度の範囲に正規化する
    /// @param a 正規化する角度　単位：度
    /// @return 因数に直接入れる
    inline void norm360P( int &a) {
        a %= 360;
        if(a < 0) a += 360;
    }

    /// @brief 範囲を比例変換する（正確な浮動小数点計算）
    /// @param min1 元の範囲の最小値
    /// @param max1 元の範囲の最大値
    /// @param min2 変換先範囲の最小値
    /// @param max2 変換先範囲の最大値
    /// @param value 変換する値
    /// @return 変換された値
    inline static float scaleRange(float min1, float max1, float min2, float max2, float value) {
        // 範囲チェック
        if(value <= min1) return min2;
        if(value >= max1) return max2;
        
        // 比例計算: (value - min1) / (max1 - min1) * (max2 - min2) + min2
        return (value - min1) / (max1 - min1) * (max2 - min2) + min2;
    }

    //誤差測定　差分の絶対値を返す
    inline static int getErr( int a, int b) { int d = abs((a - b) % 360); return (d > 180) ? (360 - d) : d; }

    inline bool diff_signs(int a, int b) {
        return (a > 0 && b < 0) || (a < 0 && b > 0);
    }

    inline bool isFront(int angle) {
        angle = norm360(angle);
        return (angle <=90||angle>=270);
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
        BAD_LINE = 6,
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



