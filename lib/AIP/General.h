#pragma once

#include <Arduino.h>
#include "Timer.h"

class General {
    public:
        void setup(); //初期設定
        int startup(); //始動処理
        int get_mode(); //現在のモードを取得

    private:
        byte startcord = 0;
        byte tact_pressed; //押されたスイッチ
        bool toggle_stat; //トグルスイッチの状態
        short mode = 0; //機体モード
        // short pNUM = 0;
        short phase = 0; //設定フェーズ
        //スタート座標　通常　左奥　右奥　右前　左前　
        int startcords_x[5] = {0, -145, 145, 145, -145}; //スタート位置のx座標
        int startcords_y[5] = {0, 257, 257, -257, -257}; //スタート位置のy座標
        bool standby = 0;
        short startPIXELs[5] = {99, 14, 2, 6, 10};
        float lastbuzzer;
        // inline void readCommand();
        Timer timer_startup;
};