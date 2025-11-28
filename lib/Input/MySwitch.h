#pragma once

#include <Arduino.h>

class MySWITCH {
    public:
        /// @brief スイッチの初期設定を行う
        void setup();
        /// @brief タクトスイッチの状態を確認する
        /// @return 左1 中央5 右9 反応なし0
        int check_tact();
        /// @brief トグルスイッチの状態を確認する
        /// @return 0:奥 1:手前
        int check_toggle();

    private:
        byte pushed_tact;
        byte toggle_stat = 0;
        const byte tactswitchPIN[3] = {33, 35, 12};
        const byte toggleswitchPIN = 10;
};
