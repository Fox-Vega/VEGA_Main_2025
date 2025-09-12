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
        // timer.reset();
        // while(digitalRead(ballPINs[i]) == HIGH && timer.read_micro() < 833);
        ballvalues[i] = pulseIn(ballPINs[i], LOW, 1666);
        if (ballvalues[i] > max_ballvalue) { //最大値の記録
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


    for (int i = 2; i > 0; i--) { //ずらす
        int a = i - 1;
        history_x[i] = history_x[a];
        history_y[i] = history_y[a];
    }
    history_x[0] = ball_x_;
    history_y[0] = ball_y_;

    total_x = 0;
    total_y = 0;
    for (int i = 0; i < 3; i++) {
        total_x += history_x[i];
        total_y += history_y[i];
    }
    ball_x = total_x / 3;
    ball_y = total_y / 3;

    // //TODO
    // for (int i = 0; i < 16; i++) {
    //     Serial.print(ballvalues[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();
}

bool BALL::get_stat() {
    return ball;
}

int BALL::get_azimuth() {
    return myvector.get_azimuth(-ball_x, -ball_y); //反転
}

int BALL::get_magnitude() {
    return myvector.get_magnitude(-ball_x, -ball_y); //反転
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
