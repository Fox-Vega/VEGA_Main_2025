#pragma once

#include <Arduino.h>

class Test {
    public:
        void test_();
        void input();
        void processing();
        void motor();
        void attitudeControl();

    private:
        uint8_t AT_Deadband = 1;//姿勢制御の±許容誤差範囲
        bool exit = false;
        bool lastPixelState = false;
        byte t_mode = 1;
        byte serial_mode = 0;
        byte motor_mode = 0;
        byte old_motor_mode = 0;
        byte motor_speed = 0;
        byte old_motor_speed = 0;
        byte ready = 0;
        float lastbuzzer = 0;
        const byte motor_PIN1[4] = {9, 7, 5, 3};
        const byte motor_PIN2[4] = {8, 6, 4, 2};
};