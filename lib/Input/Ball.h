#pragma once

#include <Arduino.h>

class BALL {
    public:
        /// @brief ボールセンサーの初期設定を行う
        void setup();
        /// @brief ボールセンサーのデータを読み取る
        void read();
        /// @brief ボール検出状態を取得する
        /// @return true:検出 false:非検出
        bool get_stat();
        /// @brief ボールの方向を取得する
        /// @return 方位角（0-359度）
        int get_azimuth();
        /// @brief ボール信号の強度を取得する
        /// @return 信号強度
        int get_magnitude();
        /// @brief ボールのX座標を取得する
        /// @return X座標
        int get_x();
        /// @brief ボールのY座標を取得する
        /// @return Y座標
        int get_y();
        /// @brief 指定されたボールセンサーの生値を取得する
        /// @param ballNUM センサー番号
        /// @return センサー値
        int get_value(byte ballNUM);

        void ss(int s);//debug

    private:
        int sss=60;
        bool ball; //検出ステータス
        int ball_x; //最終座標
        int ball_y; //最終座標
        int ball_x_; //RAW座標
        int ball_y_; //RAW座標
        int total_x = 0;
        int total_y = 0;
        int ballvalues[16]; //ボールの値を格納する配列
        byte ballNUMstart;
        byte max_ballNUM = 0;
        int max_ballvalue = 0;
        int NUMball;
        int history_x[3];
        int history_y[3];
        int history_size;
        unsigned long ball_start;
        const int ballPINs[16] = {31, 29, 27, 25, 23, 19, 17, 15, 32, 34, 36, 38, 40, 42, 44, 46};
        const float balldirs[16] = {0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5, 180, 202.5, 225, 247.5, 270, 292.5, 315, 337.5}; //ボールセンサの角度
};