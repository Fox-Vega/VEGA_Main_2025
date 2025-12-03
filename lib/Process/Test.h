#pragma once

#include <Arduino.h>

class Test {
    public:
        void test_(); //メイン処理
        void input(); //ボール、ジャイロ、タクトスイッチ
        void escape(); //ライン
        void motor(); //モーター
        void stabilize(); //姿勢制御
        void free(); //なんでも

    private:
        int modeNUM = 6;
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