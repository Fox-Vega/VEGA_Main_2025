#pragma once

#include <Arduino.h>
#include <math.h>

class LINE {
    public:
        void setup();
        void read();
        int get_value(byte lineNUM);
        int get_azimuth();
        int get_magnitude();
        int get_x();
        int get_y();
        int get_type();

    private:
        const int detection_border = 600;

        int total_x;
        int total_y;

        int line_x;
        int line_y;
        int oldline_x;
        int oldline_y;
        int escape_x;
        int escape_y;

        int line_type;
        //0なら反応なし
        //1なら線検知
        //2なら角検知

        bool trip = false;
        bool over = false;

        const int line_r = 1000;
        const uint8_t selectPIN[3] = {22, 24, 26};
        const uint8_t outputPIN[3] = {A9, A11, A13};

        int pack_x[4];
        int pack_y[4];
        int line_values[24]; //アナログ値　UI用に使う
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
        const int line_degs[24] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
};
