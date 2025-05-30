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
        bool stabilize = 0;
        float lastbuzzer;
};
