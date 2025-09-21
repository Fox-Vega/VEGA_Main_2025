#pragma once

#include <Arduino.h>

class Attack {
    public:
        void setup();
        void attack_();

    private:
        const int wrap_speed = 200;
        const int avoid_speed = 220; //ライン回避
        const int back_speed = 140;
        unsigned long back_border = 1000;

        // {},
        const float wrap[5][2] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0},
            {15, 1.0},
            {40, 1.15},
            {110, 1.7},
            {180, 1.35}
        };

        bool back = false;
        float ball_azimuth;
        float movement_azimuth;
        int wrap_size;
        unsigned long back_start;
};