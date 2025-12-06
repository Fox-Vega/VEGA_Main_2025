#pragma once

#include <Arduino.h>

class Attack {
    public:
        void setup(); //初期設定
        void attack_(); //実行

    private:
        const int avoid_speed = 190; //ライン回避
        const int back_speed = 130;
        unsigned long back_border = 1000;

        const float wrap[7][3] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0, 200},
            {10, 999, 200},
            {40, 1.05, 200},
            {60, 1.15, 190},
            {70, 1.45, 190},
            {150, 1.55, 200},
            {180, 1.30, 210}
        };

        int speed = 0;
        bool back = false;
        int ball_azimuth;
        int movement_azimuth;
        int wrap_size;
        unsigned long back_start;
};