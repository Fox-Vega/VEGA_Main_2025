#include "line.h"

void LINE::setup(void) {
    pinMode(selectPIN[0], OUTPUT);
    pinMode(selectPIN[1], OUTPUT);
    pinMode(selectPIN[2], OUTPUT);
    pinMode(outputPIN[0], INPUT);
    pinMode(outputPIN[1], INPUT);
    pinMode(outputPIN[2], INPUT);
}

void LINE::read() {
    if (byte i = 0; i < 3; i++) {
        pack_degs[i] = 0;
    }
    for (int i = 0; i < 23; i++) {
        line_stat_[i] = 0;
        line_stat[i] = 0;
    }
    for (int j = 0; j < 2; j++) { 
        for (int i = 0; i < 7; i++) {
            if (Reader[i][0] == 0) {
                digitalWrite(selectPIN[0], LOW);
            } else {
                digitalWrite(selectPIN[0], HIGH);
            }
            if (Reader[i][1] == 0) {
                digitalWrite(selectPIN[1], LOW);
            } else {
                digitalWrite(selectPIN[1], HIGH);
            }
            if (Reader[i][2] == 0) {
                digitalWrite(selectPIN[2], LOW);
            } else {
                digitalWrite(selectPIN[2], HIGH);
            }

            if (analogRead(j) > detection_border) {
                byte j_ = j;
                byte i_ = i;
                line_stat_[(j * 8) + i] += 1;
                if (line_stat_[(j * 8) + i] == 2) {
                    line_stat[(j * 8) + i] = 1;
                }
            }
        }
    }
    int total_x = 0;
    int total_y = 0;
    bool pack_NOW = 0;
    byte pach_NUM = 0;

    byte startNUM = 99;
    for (byte i = 23; i > 0; i--) {
        if (line_stat[i] == 0 && startNUM == 99) {
            startNUM = i + 1;
            break;
        }
    }
    for (byte i = startNUM; i < startNUM + 23; i++) {
        byte pLine = i % 24;
        if (line_stat[pLine] == 1) {
            myvector.get_cord(line_degs[pLine], line_r);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
            if (pack_NOW == 0) {
                pack_NOW = 1;
            }
        } else {
            if (pack_NOW == 1) {
                pack_NOW = 0;
                pack_degs[pack_NUM] = myvector.get_azimuth(total_x, total_y); 
                pack_NUM += 1;
                total_x = 0;
                total_y = 0;
            }
        }
    }
}
