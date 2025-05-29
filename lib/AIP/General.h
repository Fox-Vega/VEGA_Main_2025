#pragma once

#include <Arduino.h>

class General {
    public:
        void setup(); //初期設定
        void startup(); //設定
        int get_run(); //トグルスイッチを確認し、実行していいかを確認
        int get_mode(); //モードを確認

    private:
        int step_num = 0;
        byte startcord = 0;
        bool toggle_stat; //トグルスイッチの状態
        bool Run = false; //実行するかどうか
        short mode = 0; //機体モード
        short pNUM = 0;
        short phase = 0; //設定フェーズ
        short color[3] = {1, 1, 1};
        //スタート座標　通常　左奥　右奥　右前　左前　
        int startcords_x[5] = {1, 1, 1, 1, 1}; //スタート位置のx座標
        int startcords_y[5] = {1, 1, 1, 1, 1}; //スタート位置のy座標
        short startPIXELs[5] = {99, 14, 2, 6, 10};
        short switch_pressed; //押されたスイッチ
        float lastbuzzer;
};
