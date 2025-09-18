#pragma once

#include <Arduino.h>

class Attack {
    public:
        void setup();
        void attack_();

    private:
        const int wrap_speed = 170;
        const int avoid_speed = 200; //ライン回避
        const int back_speed = 50;

        // {},
        const float wrap[4][2] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0},
            {10, 1.0},
            {90, 1.0},
            {180, 1.0}
        };

        float ball_azimuth;
        float movement_azimuth;
        int wrap_size;
};