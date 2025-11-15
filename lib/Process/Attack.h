#pragma once

#include <Arduino.h>

class Attack {
    public:
        /// @brief Attackクラスの初期設定を行う
        void setup();
        /// @brief Attackのメイン処理ループ
        void attack_();

    private:
        const int wrap_speed = 190;
        const int avoid_speed = 210; //ライン回避
        const int back_speed = 110;
        unsigned long back_border = 1200;

        const float wrap[6][3] = {//最初は0,0必須　最低2セット　2つ目に進む方向
            {0, 0, 200},
            {15, 1.0, 200},
            {60, 2.0, 200},
            {100, 1.9, 200},
            {170, 1.3, 200},
            {180, 1.4, 200}
        };

        int speed = 0;
        bool back = false;
        int ball_azimuth;
        int movement_azimuth;
        int wrap_size;
        unsigned long back_start;
};