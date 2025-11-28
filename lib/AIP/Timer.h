#pragma once

#include <Arduino.h>

class Timer {
    public:
        /// @brief タイマーをリセットする
        void reset();
        /// @brief 経過時間を取得する（ミリ秒）
        /// @return 経過時間（1/1000秒単位）
        unsigned long read_milli();
        /// @brief 経過時間を取得する（マイクロ秒）
        /// @return 経過時間（1/1000000秒単位）
        unsigned long read_micro();

    private:
        unsigned long milliseconds;
        unsigned long microseconds;
};
