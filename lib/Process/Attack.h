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

        const float wrap[6][3] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0, 200},
            {20, 999, 210},
            {50, 1.22, 200},
            {75, 1.4, 200},
            {140, 1.58, 210},
            {180, 1.25, 210}
        };

        int speed = 0;
        bool back = false;
        int ball_azimuth;
        int movement_azimuth;
        int wrap_size;
        unsigned long back_start;
};