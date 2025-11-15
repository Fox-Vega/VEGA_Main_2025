#pragma once

#include <Arduino.h>
#include <math.h>

class LINE {
    public:
        /// @brief ラインセンサーの初期設定を行う
        void setup();
        /// @brief ラインセンサーのデータを読み取る
        void read();
        /// @brief ラインの方位角を取得する
        /// @return 方位角（0-359度）
        int get_azimuth();
        /// @brief ラインの大きさ（強度）を取得する
        /// @return 大きさ
        int get_magnitude();
        /// @brief ラインのX座標を取得する
        /// @return X座標
        int get_x();
        /// @brief ラインのY座標を取得する
        /// @return Y座標
        int get_y();
        /// @brief ラインの種類を取得する
        /// @return 0=無し 1=点 2=辺 3=角
        int get_type();
        /// @brief ライン回避方向を取得する
        /// @return 逃げる方向
        int get_eazimuth();
        /// @brief 指定されたラインセンサーの生値を取得する
        /// @param lineNUM センサー番号
        /// @return センサー値
        int get_value(byte lineNUM);
        /// @brief 指定されたラインセンサーの検出状態を取得する
        /// @param lineNUM センサー番号
        /// @return true:検出 false:非検出
        bool get_stat(byte lineNUM);
        /// @brief センサーパックの値を取得する
        /// @param packNUM パック番号（0が1つ目）
        /// @return パック値
        int get_pack(byte packNUM);

    private:
        const int detection_border = 990;//700;   990くさしお
        const int over_border = 150;

        float total_x;
        float total_y;

        int line_x;
        int line_y;
        int oldline_x;
        int oldline_y;
        int escape_x;
        int escape_y;

        int line_type;
        //0なら反応なし
        //1なら点検知
        //2なら辺検知
        //3なら角検知

        bool over = false; //反転したか

        const int line_r = 12;
        const uint8_t selectPIN[3] = {22, 24, 26};
        const uint8_t outputPIN[3] = {A9, A11, A13};

        float pack_x[4];
        float pack_y[4];
        int line_values[24]; //アナログ値　UI用
        int line_stat[24];
        int line_stat_[24];

        const byte Reader[8][3] = {
            {1, 1, 1},
            {1, 1, 0},
            {1, 0, 1},
            {1, 0, 0},
            {0, 1, 1},
            {0, 1, 0},
            {0, 0, 1},
            {0, 0, 0},
        };
        const int line_degs[24] = {15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345, 0};
};
