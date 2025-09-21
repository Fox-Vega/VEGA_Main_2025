#pragma once

#include <Arduino.h>

class BALL {
    public:
        void setup(); //初期設定
        void read(); //センサーデータ取得
        bool get_stat();
        int get_azimuth(); //ボールの方向を取得する関数
        int get_magnitude();
        int get_x();
        int get_y();
        int get_value(byte ballNUM);

    private:
        bool ball;
        int ball_x;
        int ball_y;
        int ball_x_;
        int ball_y_;
        int total_x = 0;
        int total_y = 0;
        int ballvalues[16]; //ボールの値を格納する配列
        byte ballNUMstart;
        byte max_ballNUM = 0;
        int max_ballvalue = 0;
        int NUMball;
        int history_x[7];
        int history_y[7];
        int history_size;
        unsigned long ball_start;
        const int ballPINs[16] = {31, 29, 27, 25, 23, 19, 17, 15, 32, 34, 36, 38, 40, 42, 44, 46};
        const float balldirs[16] = {0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5, 180, 202.5, 225, 247.5, 270, 292.5, 315, 337.5}; //ボールセンサの角度
};
