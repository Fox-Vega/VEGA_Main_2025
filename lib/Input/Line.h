#pragma once

#include <Arduino.h>
#include <math.h>

class LINE {
    public:
        void setup();
        void read();
        int get_azimuth(); //方位角
        int get_magnitude(); //大きさ
        int get_x();
        int get_y();
        int get_type(); //種類　0=無し　1=点　2=辺　3=角
        int get_eazimuth(); //逃げる方向
        int get_value(byte lineNUM);
        bool get_stat(byte lineNUM);
        int get_pack(byte packNUM); //0が1つ目

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
