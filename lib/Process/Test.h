#pragma once

#include <Arduino.h>

class Test {
    public:
        /// @brief テストモードのメイン処理
        void test_();
        /// @brief 入力センサーのテスト
        void input();
        /// @brief ライン回避のテスト
        void escape();
        /// @brief モーター動作のテスト
        void motor();
        /// @brief 姿勢制御のテスト
        void stabilize();
        /// @brief モーターフリー状態のテスト
        void free();

    private:
        int modeNUM = 6;
        uint8_t AT_Deadband = 1;//姿勢制御の±許容誤差範囲
        bool exit = false;
        bool lastPixelState = false;
        byte t_mode = 1;
        byte serial_mode = 0;
        byte motor_mode = 0; //1正転　2フリー　3逆転
        byte old_motor_mode = 0;
        byte motor_speed = 0;
        byte old_motor_speed = 0;
        byte ready = 0;
        float lastbuzzer = 0;
        const byte motor_PIN1[4] = {9, 7, 5, 3};
        const byte motor_PIN2[4] = {8, 6, 4, 2};
        int crr;
};