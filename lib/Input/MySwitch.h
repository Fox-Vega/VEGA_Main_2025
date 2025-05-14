#include <Arduino.h>

class MySWITCH {
    public:
        void setup();
        int check_tact();//タクトスイッチの状態を確認（右側が優先）
        int check_toggle();//トグルスイッチの状態を確認

    private:
        byte pushed_tact;
        byte toggle_stat = 0;
        const byte tactswitchPIN[3] = {12, 35, 33};
        const byte toggleswitchPIN = 10;
};
