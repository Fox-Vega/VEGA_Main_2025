#pragma once
#include <Arduino.h>
#include <Timer.h>

// ちなpはプロセスの略

class Defense {
public:
    void setup(void);                  // 初期設定
    void defense_(void);               // メイン処置
    // bool useSerial(bool use);          // シリアルを使うかどうか コマンドで変える
    // void timerReset();
private:
    const int dash_border=8000;
    const int ball_move_border=7;
    const int ball_power=180;//ball
    const float line_late=0.9;

    int lastdetect;
    int move_azimuth;
    int move_power;
    double move_x;
    double move_y;
    bool ball_move=false;
    Timer Dtimer;

    inline int norm360(int a) { a %= 360; if(a < 0) a += 360; return a; }
    inline bool diff_signs(int a, int b) {return (a >= 0 && b < 0) || (a < 0 && b >= 0);}
        // --- NeoPixel ---
    // struct RGBA {
    //     int red;
    //     int green;
    //     int blue;
    //     float alpha;
    // };

};