#pragma once

#include <Arduino.h>

class MySWITCH {
    public:
        void setup();
        int check_tact();//タクトスイッチの状態を確認（右側が優先）　反応なしなら０
        int check_toggle();//トグルスイッチの状態を確認 0が奥、1が手前　反応なしなら０

    private:
        byte pushed_tact;
        byte toggle_stat = 0;
        const byte tactswitchPIN[3] = {33, 35, 12};
        const byte toggleswitchPIN = 10;
};
