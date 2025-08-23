#include "Ball.h"
#include "Input.h"
#include "Output.h"
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


    timer.reset();

    // センサー値の取得
    while (timer.read_micro() < (833 * 4)) {
    // for (int i = 0; i < 100; i++) {
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
    ball_azimuth = myvector.get_azimuth(total_x, total_y);
    if (value[max_ballNUM] != 0) {
        mypixel.closest(ball_azimuth, 255, 50, 50, 1);
    }

    //記録更新
    for (int i = filter_size - 1; i > 0; i--) {
        history[i] = history[i - 1];
    }
    if (value[max_ballNUM] != 0) {
        history[0] = value[max_ballNUM];
    } else {
        history[0] = 0;
    }
}

int BALL::get_value(short ballNUM) {
    byte sensorNUM = ballNUM;
    if (ballNUM == 99) {
        sensorNUM = max_ballNUM;
    }
    if (ballvalues[sensorNUM] < detection_border) {
        ballvalues[sensorNUM] = 0;
    } else {
        value[sensorNUM] = ballvalues[sensorNUM] * ballvalue_offset;
    }
    return value[sensorNUM];
}

int BALL::get_magnitude() {
    int sum = 0;
    for (int i = 0; i < filter_size; i++) {
        sum += history[i];
    }

    int magnitude = sum / filter_size;
    magnitude = max_value - magnitude;
    old_magnitude = magnitude; // 過去の値を更新

    return magnitude;
}

int BALL::get_azimuth() {
    return ball_azimuth;
}