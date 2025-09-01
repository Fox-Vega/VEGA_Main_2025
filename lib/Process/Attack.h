#pragma once

#include <Arduino.h>

class Attack {
    public:
        void attack_();

        void normal();
    private:
        
        const short avoid_speed = 100; //ライン回避


        short ball_dir;
        short face;
        int movedir;
};
