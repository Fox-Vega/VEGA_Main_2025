#include "Ball.h"
#include "Input.h"
#include "AIP.h"

void BALL::setup() {
    for (int i = 0; i < NUMball; i++) {
        pinMode(ballPINs[i], INPUT);
    }
}

void BALL::read() {
    Serial.println("1");
    total_x = 0;
    total_y = 0;
    max_ballvalue = 0;
    // ballvalues の初期化
    for (int i = 0; i < NUMball; i++) {
        ballvalues[i] = 0;
    }
    Serial.println("2");
    // センサー値の取得
    for (int j = 0; j < 25; j++) {
        for (int i = 0; i < NUMball; i++) {
            if (digitalRead(ballPINs[i]) == LOW) {
                ballvalues[i]++;
            }
        }
    }
    Serial.println("3");
    // 最大値の探索
    for (int i = 0; i < NUMball; i++) {
        if (ballvalues[i] > max_ballvalue) {
            max_ballvalue = ballvalues[i];
            max_ballNUM = i;
        }
    }
    Serial.println("4");
    // ballNUMstart の補正
    int ballNUMstart = max_ballNUM - 2;
    if (ballNUMstart < 0) {
        ballNUMstart += NUMball;
    }
    Serial.println("5");
    // 座標計算
    for (int i = 0; i < 5; i++) {
        int ballNUM = (ballNUMstart + i) % NUMball; // 自動循環処理

        myvector.get_cord(balldirs[ballNUM], ball.get_value(ballNUM));
        total_x += myvector.get_x();
        total_y += myvector.get_y();
    }
    Serial.println("6");
}

int BALL::get_value(short ballNUM) { 
    if (ballvalues[ballNUM] < detection_border) {
        ballvalues[ballNUM] = 0;
    }
    Serial.println(ballvalues[ballNUM]);
    return ballvalues[ballNUM] * ballvalue_offset;
}

int BALL::get_magnitude() {
    Serial.println(ballvalues[max_ballNUM] * ballvalue_offset);
    return ballvalues[max_ballNUM] * ballvalue_offset;
}

int BALL::get_azimuth() {
    Serial.println(myvector.get_azimuth(total_x, total_y));
    return myvector.get_azimuth(total_x, total_y);
}