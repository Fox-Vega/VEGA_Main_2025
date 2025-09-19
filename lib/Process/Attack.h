#pragma once

#include <Arduino.h>

class Attack {
    public:
        void setup();
        void attack_();

    private:
        const int wrap_speed = 190;
        const int avoid_speed = 190; //ライン回避
        const int back_speed = 130;
        unsigned long back_border = 1000;

        // {},
        const float wrap[5][2] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0},
            {15, 1.0},
            {40, 1.38},
            {90, 1.5},
            {180, 1.4}
        };

        bool back = false;
        float ball_azimuth;
        float movement_azimuth;
        int wrap_size;
        unsigned long back_start;
};