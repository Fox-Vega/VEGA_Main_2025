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
    // ballvalues の初期化
    for (int i = 0; i < NUMball; i++) {
        ballvalues[i] = 0;
    }

    // センサー値の取得
    for (int j = 0; j < 25; j++) {
        for (int i = 0; i < NUMball; i++) {
            if (digitalRead(ballPINs[i]) == LOW) {
                ballvalues[i]++;
                if (ballvalues[i] > max_ballvalue) { //最大値の記録
                    max_ballvalue = ballvalues[i];
                    max_ballNUM = i;
                }
            }
        }
    }

    // ballNUMstart の補正
    int ballNUMstart = (max_ballNUM + 14) % 16;
    
    // 座標計算
    for (int i = 0; i < 5; i++) {
        int ballNUM = (ballNUMstart + i) % NUMball;

        myvector.get_cord(balldirs[ballNUM], ball.get_value(ballNUM));
        total_x += myvector.get_x();
        total_y += myvector.get_y();
    }
}

int BALL::get_value(short ballNUM) { 
    if (ballvalues[ballNUM] < detection_border) {
        ballvalues[ballNUM] = 0;
    } else {
        value[ballNUM] = filterCoefficient * ballvalues[ballNUM] + (1 - filterCoefficient) * old_value[ballNUM];
        value[ballNUM] = (25 / value[ballNUM]) * ballvalue_offset;
        old_value[ballNUM] = value[ballNUM];
    }
    return value[ballNUM];
}

int BALL::get_magnitude() {
    return value[max_ballNUM];
}

int BALL::get_azimuth() {
    return myvector.get_azimuth(total_x, total_y);
}
