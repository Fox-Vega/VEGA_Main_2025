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
}
