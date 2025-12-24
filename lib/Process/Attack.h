#pragma once

#include <Arduino.h>

class Attack {
    public:
        void setup(); //初期設定
        void attack_(); //実行

    private:
        const int avoid_speed = 230; //ライン回避
        const int back_speed = 140;
        unsigned long back_border = 800;

        const float wrap[6][3] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0, 200},
            {15, 999, 210},
            {40, 1.25, 210},
            {80, 1.35, 210},
            {140, 1.45, 210},
            {180, 1.30, 210}
        };

        bool near;
        bool side; //回り込みの向き
        bool turn; //ゴール向くか
        int speed = 0;
        bool back = false;
        int ball_azimuth;
        int movement_azimuth;
        int wrap_size;
        unsigned long back_start;
};