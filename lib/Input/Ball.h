#pragma once

#include <Arduino.h>

class BALL {
    public:
        void setup(); //初期設定
        void read(); //センサーデータ取得
        int get_value(short ballNUM); //ボール値を取得する関数　99を使うと最大値を取得
        int get_magnitude(); //ボールの距離を取得する関数
        int get_azimuth(); //ボールの方向を取得する関数

    private:
        //調整用
        const int ballvalue_offset = 30;
        const int detection_border = 3; //反応ボーダー
        const int max_value = 30 * ballvalue_offset;

        const int filter_size = 20;
        int history[20] = {0}; //フィルターサイズを変更したら配列数も変更。


        int value[16];
        int total_x = 0;
        int total_y = 0;
        int ballvalue;
        int ball_azimuth;
        int old_magnitude;
        int ballvalues[16]; //ボールの値を格納する配列
        int total_magnitude = 0;
        byte ballNUMstart;
        byte max_ballNUM = 0;
        byte max_ballvalue = 0;
        const byte NUMball = 16;
        const int ballPINs[16] = {31, 29, 27, 25, 23, 19, 17, 15, 32, 34, 36, 38, 40, 42, 44, 46};
        const float balldirs[16] = {0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5, 180, 202.5, 225, 247.5, 270, 292.5, 315, 337.5}; //ボールセンサの角度
};