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
    total_azimuth = 0;
    total_magnitude = 0;
    for (int i = 0; i < data_stabilizer; i++) {
        x = 0;
        y = 0;
        max_ballvalue = 0;

        //ボールセンサーデータの初期化
        for (int i = 0; i < NUMball; i++) {
            ballvalues[i] = 0;
        }

        //センサー値の取得
        for (int j = 0; j < 25; j++) {
            for (int i = 0; i < NUMball; i++) {
                if (digitalRead(ballPINs[i]) == LOW) {
                    ballvalues[i]++;
                }
            }
        }
        
        //最大値の取得
        for (int i = 0; i < NUMball; i++) {
            if (ballvalues[i] > max_ballvalue) {
                max_ballvalue = ballvalues[i];
                max_ballNUM = i;
            }
        }

        //ベクトル処理開始センサーの補正
        int ballNUMstart = max_ballNUM - 2;
        if (ballNUMstart < 0) {
            ballNUMstart += NUMball;
        }

        //座標計算
        for (int i = 0; i < 5; i++) {
            int ballNUM = (ballNUMstart + i) % NUMball;
            if (ballvalues[ballNUM] < detection_border) {
                ballvalues[ballNUM] = 0;
            }
            myvector.get_cord(balldirs[ballNUM], ballvalues[ballNUM] * ballvalue_offset);
            x += myvector.get_x();
            y += myvector.get_y();
        }
        total_x += x; //今のところ使い道無し
        total_y += y; //今のところ使い道無し
        total_azimuth += myvector.get_azimuth(x, y);
        total_magnitude += ballvalues[max_ballNUM] * ballvalue_offset;
    }
    azimuth = total_azimuth / data_stabilizer;
    magnitude = total_magnitude / data_stabilizer;
}

int BALL::get_magnitude() {
    return azimuth;
}

int BALL::get_azimuth() {
    return magnitude;
}