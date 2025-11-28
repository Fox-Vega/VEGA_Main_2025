#pragma once

#include <Arduino.h>
#include "Timer.h"

class General {
    public:
        /// @brief Generalクラスの初期設定を行う
        void setup();
        /// @brief 起動時の設定処理を行う
        void startup();
        /// @brief トグルスイッチを確認し実行可否を取得する
        /// @return 実行可否
        int get_run();
        /// @brief 現在のモードを取得する
        /// @return モード番号
        int get_mode();

    private:
        // int step_num = 0;
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