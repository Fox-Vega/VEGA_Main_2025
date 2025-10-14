#pragma once

#include <Arduino.h>

class Attack {
    public:
        void setup();
        void attack_();

    private:
        const int wrap_speed = 190;
        const int avoid_speed = 230; //ライン回避
        const int back_speed = 110;
        unsigned long back_border = 1200;

        // {},
        const float wrap[5][3] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0, 200},
            {90, 1.0, 200},
            {140, 1.35, 200},
            {170, 1.4, 200},
            {180, 1.5, 200}
        };

        int speed = 0;
        bool back = false;
        float ball_azimuth;
        float movement_azimuth;
        int wrap_size;
        unsigned long back_start;
};