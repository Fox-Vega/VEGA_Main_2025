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
    Serial.print(">Ball1:");
    Serial.println(ballvalues[0]);
    Serial.print(">Ball2:");
    Serial.println(ballvalues[1]);
    Serial.print(">Ball3:");
    Serial.println(ballvalues[2]);
    Serial.print(">Ball4:");
    Serial.println(ballvalues[3]);
    Serial.print(">Ball5:");
    Serial.println(ballvalues[4]);
    Serial.print(">Ball6:");
    Serial.println(ballvalues[5]);
    Serial.print(">Ball7:");
    Serial.println(ballvalues[6]);
    Serial.print(">Ball8:");
    Serial.println(ballvalues[7]);
    Serial.print(">Ball9:");
    Serial.println(ballvalues[8]);
    Serial.print(">Ball10:");
    Serial.println(ballvalues[9]);
    Serial.print(">Ball11:");
    Serial.println(ballvalues[10]);
    Serial.print(">Ball12:");
    Serial.println(ballvalues[11]);
    Serial.print(">Ball13:");
    Serial.println(ballvalues[12]);
    Serial.print(">Ball14:");
    Serial.println(ballvalues[13]);
    Serial.print(">Ball15:");
    Serial.println(ballvalues[14]);
    Serial.print(">Ball16:");
    Serial.println(ballvalues[15]);
    
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
    for (int i = 0; i < 5; i++) {
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