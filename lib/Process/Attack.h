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
            {15, 999, 200},
            {50, 1.1, 190},
            {90, 1.35, 160},
            {155, 1.5, 180},
            {180, 1.4, 210}
        };

        int speed = 0;
        bool back = false;
        int ball_azimuth;
        int movement_azimuth;
        int wrap_size;
        unsigned long back_start;
};