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
            }
        }
    }

    // 最大値の探索
    for (int i = 0; i < NUMball; i++) {
        if (ballvalues[i] > max_ballvalue) {
            max_ballvalue = ballvalues[i];
            max_ballNUM = i;
        }
    }

    // ballNUMstart の補正
    int ballNUMstart = max_ballNUM - 2;
    if (ballNUMstart < 0) {
        ballNUMstart += NUMball;
    }

    // 座標計算
    for (int i = 0; i < 4; i++) {
        int ballNUM = (ballNUMstart + i) % NUMball; // 自動循環処理

        myvector.get_cord(balldirs[ballNUM], ball.get_value(ballNUM));
        total_x += myvector.get_x();
        total_y += myvector.get_y();
    }
}

int BALL::get_value(short ballNUM) { 
    if (ballvalues[ballNUM] < detection_border) {
        ballvalues[ballNUM] = 0;
    }
    return ballvalues[ballNUM] * ballvalue_offset;
}

int BALL::get_magnitude() {
    return ballvalues[max_ballNUM] * ballvalue_offset;
}

int BALL::get_azimuth() {
    return myvector.get_azimuth(total_x, total_y);
}
