#include <Arduino.h>

class Timer {
    public:
        void reset();
        unsigned long read_milli(); //時間を取得(1/1000s)
        unsigned long read_micro(); //時間を取得(1/1000000s)

    private:
        unsigned long milliseconds;
        unsigned long microseconds; 
};
