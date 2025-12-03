#pragma once

#include <Arduino.h>

class Attack {
    public:
        void setup(); //初期設定
        void attack_(); //実行

    private:
        const int avoid_speed = 190; //ライン回避
        const int back_speed = 110;
        unsigned long back_border = 1200;

        const float wrap[6][3] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0, 200},
            {15, 999, 210},
            {30, 1.15, 210},
            {70, 1.3, 210},
            {160, 1.4, 210},
            {180, 1.3, 210}
        };

        int speed = 0;
        bool back = false;
        int ball_azimuth;
        int movement_azimuth;
        int wrap_size;
        unsigned long back_start;
};