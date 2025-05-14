#include "Ball.h"
#include "Input.h"
#include "AIP.h"

void BALL::setup() {
    for (int i = 0; i < NUMball; i++) {
        pinMode(ballPINs[i], INPUT);
    }
}

void BALL::read() {
    total_x = 0;
    total_y = 0;
    max_ballvalue = 0;
    for (int i = 0; i < 16; i++) {
        ballvalues[i] = 0;
    }
    for (int j = 0; j < 60; j++) {
        for (int i = 0; i < NUMball; i++) {
            if (digitalRead(ballPINs[i]) == HIGH) {
                ballvalues[i] = ballvalues[i] + 1;
            }
        }
    }
    for (int i = 0; i < NUMball; i++) {
        if (ballvalues[i] > max_ballvalue) {
            max_ballvalue = ballvalues[i];
            max_ballNUM = i;
        }
    }
    int ballNUMstart = max_ballNUM - 3;
    if (ballNUMstart < 0) {
        ballNUMstart += 16;
    }
    for (int i = ballNUMstart; i < ballNUMstart + 6; i++) {
        if (i >= NUMball) {
            i -= 16;
        }
        myvector.get_cord(balldirs[i], ball.get_value(i));
        total_x += myvector.get_x();
        total_y += myvector.get_y();
    }
}

int BALL::get_value(short ballNUM) { 
    return ballvalues[ballNUM] * ballvalue_offset;
}

int BALL::get_magnitude() {
    return myvector.get_magnitude(total_x, total_y);
}

int BALL::get_azimuth() {
    return myvector.get_azimuth(total_x, total_y);
}
