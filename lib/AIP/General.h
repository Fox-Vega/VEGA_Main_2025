#pragma once

#include <Arduino.h>

class General {
    public:
        void setup(); //初期設定
        int startup(); //設定
        int check_run(); //タクトスイッチを確認し、実行していいかを確認
        int check_mode(); //モードを確認
        void update(); //値を更新

    private:
        byte startcord = 0;
        bool toggle_stat; //トグルスイッチの状態
        bool Run = false; //実行するかどうか
        short mode = 0; //機体モード
        short phase = 0; //設定フェーズ
        //スタート座標　通常　右奥　左奥　右前　左前　
        short startcords_x[5] = {0, 0, 0, 0, 0}; //スタート位置のx座標
        short startcords_y[5] = {0, 0, 0, 0, 0}; //スタート位置のy座標
        short startPIXELs[5] = {0, 0, 0, 0, 0};
        short switch_pressed; //押されたスイッチ
};