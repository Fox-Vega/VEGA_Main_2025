#include "Ball.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"


void BALL::setup() {
    for (int i = 0; i < NUMball; i++) {
        pinMode(ballPINs[i], INPUT);
    }
    history_size = sizeof(history_x) / sizeof(history_x[0]);
    NUMball = sizeof(ballPINs) / sizeof(ballPINs[0]);
}

void BALL::read() {
    //センサー値初期化
    for (int i = 0; i < NUMball; i++) {
        ballvalues[i] = 0;
    }

    //センサー値取得
    ball_start = micros();
    while((micros() - ball_start) < (833 * 3)) {
        for (int i = 0; i < NUMball; i++) {
            if (digitalRead(ballPINs[i]) == LOW) {
                ballvalues[i]++;
            }
        }
    }

    //最大値の記録
    max_ballNUM = 99;
    max_ballvalue = 0;
    for (int i = 0; i < NUMball; i++) {
        if (ballvalues[i] > max_ballvalue) {
            max_ballvalue = ballvalues[i];
            max_ballNUM = i;
        }
    }

    if (max_ballNUM == 99) {
        ball = 0;
        ball_x_ = 0;
        ball_y_ = 0;
    } else {
        ball = 1;
        // 座標計算
        total_x = 0;
        total_y = 0;
        int ballNUMstart = (max_ballNUM + 14) % NUMball; //ベクトル移動平均計算の開始センサー番号
        for (int i = 0; i < 3; i++) {
            int ballNUM = (ballNUMstart + i) % NUMball;
            myvector.get_cord(balldirs[ballNUM], ballvalues[ballNUM]);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
        }
        ball_x_ = total_x / 3;
        ball_y_ = total_y / 3;
    }

    if (history_size != 1) {
        //ずらす
        for (int i = (history_size - 1); i > 0; i--) {
            int a = i - 1;
            history_x[i] = history_x[a];
            history_y[i] = history_y[a];
        }
        history_x[0] = ball_x_;
        history_y[0] = ball_y_;

        total_x = 0;
        total_y = 0;
        for (int i = 0; i < history_size; i++) {
            total_x += history_x[i];
            total_y += history_y[i];
        }
        ball_x = total_x / history_size;
        ball_y = total_y / history_size;
    } else {
        ball_x = ball_x_;
        ball_y = ball_y_;
    }
}

bool BALL::get_stat() {
    return ball;
}

int BALL::get_azimuth() {
    return myvector.get_azimuth(ball_x, ball_y);
}

int BALL::get_magnitude() {
    return myvector.get_magnitude(ball_x, ball_y);
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
