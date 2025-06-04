#pragma once

#include <Arduino.h>

class Test {
    public:
        void test_();
        void input();
        void cord();
        void motor();
        // void ln();
    private:
        int t_mode = 1;
        byte ready = 0;
        float lastbuzzer = 0;
        const byte motor_PIN1[4] = {9, 7, 5, 3};
        const byte motor_PIN2[4] = {8, 6, 4, 2};
};
