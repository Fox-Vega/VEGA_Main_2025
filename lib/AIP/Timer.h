#pragma once

#include <Arduino.h>

class Timer {
    public:
        inline void reset(){
            milliseconds = millis();
            microseconds = micros();
        };
        inline unsigned long read_milli(){
            return (millis() - milliseconds);
        };
        inline unsigned long read_micro(){
            return (micros() - microseconds);
        };

    private:
        unsigned long milliseconds;
        unsigned long microseconds;
};
