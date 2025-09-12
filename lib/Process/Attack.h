#pragma once

#include <Arduino.h>

class Attack {
    public:
        void setup();
        void attack_();

    private:
        const short wrap_speed = 200;
        const short avoid_speed = 200; //ライン回避
        const int wrap[1][2] = {{0, 0}}; //最初は0,0必須

        short ball_dir;
        short face;
        int movedir;
        size_t wrap_size;
};
