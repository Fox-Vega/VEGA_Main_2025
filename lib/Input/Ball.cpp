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
    //センサー値初期化
    for (int i = 0; i < NUMball; i++) {
        ballvalues[i] = 0;
    }
    //センサー値取得
    max_ballNUM = 99;
    max_ballvalue = 0;
    for (int i = 0; i < NUMball; i++) {
        timer.reset();
        ballvalues[i] = pulseIn(ballPINs[i], LOW, 833);
        if (ballvalues[i] > max_ballvalue) { //最大値の記録
            max_ballvalue = ballvalues[i];
            max_ballNUM = i;
        }
    }
    if (max_ballNUM == 99) {
        ball = false;
        ball_x_ = 0;
        ball_y_ = 0;
    } else {
        ball = true;
        // 座標計算
        total_x = 0;
        total_y = 0;
        int ballNUMstart = (max_ballNUM + 14) % NUMball; //ベクトル移動平均計算開始センサー番号
        for (int i = 0; i < 5; i++) {
            int ballNUM = (ballNUMstart + i) % NUMball;
            myvector.get_cord(balldirs[ballNUM], ballvalues[ballNUM]);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
        }
        ball_x_ = total_x / 5;
        ball_y_ = total_y / 5;
    }


    for (int i = 14; i > 0; ) {
        history_x[i - 1] = history_x[i];
        history_y[i - 1] = history_y[i];
    }
    history_x[0] = ball_x_;
    history_y[0] = ball_y_;

    total_x = 0;
    total_y = 0;
    for (int i = 0; i < 15; i++) {
        total_x += history_x[i];
        total_y += history_y[i];
    }
    ball_x = total_x / 15;
    ball_y = total_y / 15;
}

int BALL::get_stat() {
    return ball;
}

int BALL::get_azimuth() {
    return myvector.get_azimuth(-total_x, -total_y);
}

int BALL::get_magnitude() {
    return myvector.get_magnitude(-total_x, -total_y);
}

int BALL::get_x() {
    return ball_x;
}

int BALL::get_y() {
    return ball_y;
}

int BALL::get_value(byte ballNUM) {
    return ballvalues[ballNUM];
}
