#pragma once

#include <Arduino.h>

class Attack {
    public:
        void setup();
        void attack_();

    private:
        const short wrap_speed = 200;
        const short avoid_speed = 200; //ライン回避
        const short back_speed = 50;
        const float wrap[3][2] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0},
            {15, 15},
            {180, 150}
        };

        short ball_dir;
        short face;
        int movement_azimuth;
        int wrap_size;
};
