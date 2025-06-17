#pragma once

#include <Arduino.h>

class LINE {
    public:
        void setup();
        void read();
        int get_value();
        int get_azimuth();
        int get_magnitude();
    private:
        const int detection_border = 400;

        const uint8_t selectPIN[3] = {22, 24, 26};
        const uint8_t outputPIN[3] = {A11, A13, A9};
        int pack_degs[4];
        const int line_r = 11;
        int line_stat_[24];
        int line_stat[24];
        const byte Reader[8][3] = {
            {0, 0, 0}, 
            {0, 0, 1}, 
            {0, 1, 0}, 
            {0, 1, 1}, 
            {1, 0, 0}, 
            {1, 0, 1}, 
            {1, 1, 0}, 
            {1, 1, 1}, 
        };
        const int line_degs[24] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
};
