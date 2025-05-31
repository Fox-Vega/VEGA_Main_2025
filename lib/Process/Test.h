#pragma once

#include <Arduino.h>

class Test {
    public:
        void test_();
        void input();
        // void goal();
        void ir();
        // void ln();
    private:
        int t_mode = 1;
        int lastbuzzer = 0;
        const byte motor_PIN1[4] = {8, 7, 5, 3};
        const byte motor_PIN2[4] = {9, 6, 4, 2};
};
