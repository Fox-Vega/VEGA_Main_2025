#include "Ball.h"
#include "Input.h"
#include "AIP.h"

void BALL::setup() {
    for (int i = 0; i < NUMball; i++) {
        pinMode(ballPINs[i], INPUT);
    }
}

void Ball::read() {
    int azimuth = 0;
    int magnitude = 0;
    for (int i = 0; i < 2; i++) {
        ball.read_();
        azimuth += ball.get_azimuth_;
        magnitude += ball.get_magnitude_();
    }
    myvector.get_cord(azimuth / 2, magnitude / 2);
    x = myvector.get_x();
    y = myvector.get_y();
}

int Ball::get_azimuth() {
    return myvector.get_azimuth(x, y);
}

int Ball::get_worldazimuth() {
    float worldazimuth = ball.get_azimuth() + gam.get_azimuth();
    if (worldazimuth >= 360) {
        worldazimuth -= 360;
    }
    return worldazimuth;
}

int Ball::get_magnitude() {
    return myvector.get_magnitude(x , y);
}

int Ball::get_x() {
    return x;
}

int Ball::get_y() {
    return y;
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

        myvector.get_cord(balldirs[ballNUM], ball.get_value(ballNUM));
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
