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
        int get_avoid();
        int get_x();
        int get_y();
        int get_ax();
        int get_ay();
        int get_type();

    private:
        const int detection_border = 300;

        int total_x;
        int total_y;
        int point1;
        int point2;
        int point3;
        int point4;
        int point1_;
        int point2_;
        int point3_;
        int point4_;
        int line_x;
        int line_y;
        int avoid_x;
        int avoid_y;

        int line_type;

        int line_dif;
        int line2_dif;
        int line_deg;
        int line2_deg;
        int line_theta;
        int line2_theta;
        int line_dist;
        int line2_dist;

        const int line_r = 110;
        const uint8_t selectPIN[3] = {22, 24, 26};
        const uint8_t outputPIN[3] = {A9, A11, A13};

        int pack_degs[4];
        int line_values[24];
        int line_stat_[24];
        int line_stat[24];

        bool over;

        short dif[3];

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

        #define rad_to_deg (180.0 / 3.14159265)
};