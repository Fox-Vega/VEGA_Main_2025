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
    const int return_power = 200;
    int line_azimuth;
    int line_stat;
    int go_border[2];
    int ball_azimuth;
    int ball_stat;
    int r_azimuth;
    int line_ang;
    int ball_ang;
    int move_ang;
    int lastdetect;
    int calb;
    enum class FROG {
        NONE = 0,
        NORMAL = 1,     // ノーマル
        NO_LINE = 2,    // ラインなし
        NO_BALL = 3,    // ボールなし
    };
    FROG frog;

    // // === ユーティリティ関数 ===

    // //360補正

    /// @brief 360度の範囲に正規化する
    /// @param a 正規化する角度　単位：度
    /// @return 正規化された角度
    inline int norm360(int a) {
        a %= 360;
        if(a < 0) a += 360;
        return a;
    }

    /// @brief 360度の範囲に正規化する
    /// @param a 正規化する角度　単位：度
    /// @return 因数に直接入れる
    inline void norm360P(int &a) {
        a %= 360;
        if(a < 0) a += 360;
    }

    //誤差測定　差分の絶対値を返す
    inline int getErr(int a, int b) { int d = abs((a - b) % 360); return (d > 180) ? (360 - d) : d; }

    inline bool diff_signs(int a, int b) {
        return (a >= 0 && b < 0) || (a < 0 && b >= 0);
    }

    inline bool isFront(int angle) {
        angle = norm360(angle);
        return (angle <=90||angle>=270);
    }

    void to_range(int &angle, int start_angle, bool ccw) {
        angle = norm360(angle);  // まず正規化
        int end_angle = norm360(start_angle + 180);
        
        if (!ccw) { // 時計回り180度範囲
            // 360度境界をまたぐ場合（例: 270°～90°）
            if (start_angle > end_angle) {
                // 範囲内判定: angle >= start_angle OR angle <= end_angle
                if (angle < start_angle && angle > end_angle) {
                    // 範囲外なので最も近い境界に制限
                    int err_start = getErr(angle, start_angle);
                    int err_end = getErr(angle, end_angle);
                    
                    angle = (err_start < err_end) ? start_angle : end_angle;
                }
            } else {
                // 通常範囲（例: 0°～180°）
                if (angle < start_angle || angle > end_angle) {
                    // 範囲外なので最も近い境界に制限
                    int err_start = getErr(angle, start_angle);
                    int err_end = getErr(angle, end_angle);
                    
                    angle = (err_start < err_end) ? start_angle : end_angle;
                }
            }
        }
    }

};



