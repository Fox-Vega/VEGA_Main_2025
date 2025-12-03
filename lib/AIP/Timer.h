#pragma once

#include <Arduino.h>

class Timer {
    public:
        void reset();
        unsigned long read_milli();
        unsigned long read_micro();

    private:
        unsigned long milliseconds;
        unsigned long microseconds;
};
