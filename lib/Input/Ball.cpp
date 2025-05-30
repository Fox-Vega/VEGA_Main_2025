#include "Ball.h"
#include "Input.h"
#include "AIP.h"

void BALL::setup() {
    for (int i = 0; i < NUMball; i++) {
        pinMode(ballPINs[i], INPUT);
    }
}

void BALL::read() {
    float azimuth = 0.0;
    float magnitude = 0.0;
    for (int i = 0; i < 2; i++) {
        // 現在のオブジェクトの関数を呼び出す（グローバルな ball ではなく）
        this->read_();
        azimuth += this->get_azimuth_();
        magnitude += this->get_magnitude_();
    }
    // 平均値から座標計算
    myvector.get_cord(azimuth / 2, magnitude / 2);
    x = myvector.get_x();
    y = myvector.get_y();
}

int BALL::get_azimuth() {
    return myvector.get_azimuth(x, y);
}

int BALL::get_worldazimuth() {
    // 現在のオブジェクトの get_azimuth() を呼び出す
    float worldazimuth = this->get_azimuth() + gam.get_azimuth();
    if (worldazimuth >= 360) {
        worldazimuth -= 360;
    }
    return worldazimuth;
}

int BALL::get_magnitude() {
    return myvector.get_magnitude(x, y);
}

void BALL::read_() {
    x_ = 0;
    y_ = 0;
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
        int ballNUM = (ballNUMstart + i) % NUMball; // 自動循環
        // このオブジェクトの get_value() を使う
        myvector.get_cord(balldirs[ballNUM], this->get_value(ballNUM));
        x_ += myvector.get_x();
        y_ += myvector.get_y();
    }
}

int BALL::get_value(short ballNUM) { 
    if (ballvalues[ballNUM] < detection_border) {
        ballvalues[ballNUM] = 0;
    }
    return ballvalues[ballNUM] * ballvalue_offset;
}

int BALL::get_azimuth_() {
    return myvector.get_azimuth(x_, y_);
}

int BALL::get_magnitude_() {
    return ballvalues[max_ballNUM] * ballvalue_offset;
}