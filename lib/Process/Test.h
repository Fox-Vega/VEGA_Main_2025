#pragma once

#include <Arduino.h>

class Test {
    public:
        void test_();
        void input();
        // void goal();
        void motor();
        // void ln();
    private:
        int t_mode = 1;
        float lastbuzzer = 0;
        const byte motor_PIN1[4] = {9, 7, 5, 3};
        const byte motor_PIN2[4] = {8, 6, 4, 2};
};
