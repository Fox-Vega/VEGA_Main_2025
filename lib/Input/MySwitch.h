#pragma once

#include <Arduino.h>

class MySWITCH {
    public:
        void setup(); //初期設定
        int check_tact(); //タクトを確認
        int check_toggle(); //トグルを確認　０＝奥、１＝手前

    private:
        byte pushed_tact;
        byte toggle_stat = 0;
        const byte tactswitchPIN[3] = {33, 35, 12};
        const byte toggleswitchPIN = 10;
};
