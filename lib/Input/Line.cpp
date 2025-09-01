#include "Line.h"
#include "AIP.h"
#include "Input.h"

void LINE::setup(void) {
    for (int i = 0; i < 3; i++) {
        pinMode(selectPIN[i], OUTPUT);
        pinMode(outputPIN[i], INPUT);
    }
}

void LINE::read() {
    for (int i = 0; i < 24; i++) { //初期化
        line_stat[i] = 0;
        line_stat_[i] = 0;
    }

    //読み取り
    for (int k = 0; k < 2; k++) { //2回測定
        for (int j = 0; j < 3; j++) { //3つのマルチプレクサを読む
            for (int i = 0; i < 8; i++) { //8回の変更
                // selectPIN[0] (ピン22)
                // if (Reader[i][0] == 0) {
                //     digitalWrite(selectPIN[0], LOW);
                // } else {
                //     digitalWrite(selectPIN[0], HIGH);
                // }
                // selectPIN[1] (ピン24)
                // if (Reader[i][1] == 0) {
                //     digitalWrite(selectPIN[1], LOW);
                // } else {
                //     digitalWrite(selectPIN[1], HIGH);
                // }
                // selectPIN[2] (ピン26)
                // if (Reader[i][2] == 0) {
                //     digitalWrite(selectPIN[2], LOW);
                // } else {
                //     digitalWrite(selectPIN[2], HIGH);
                // }
                if (Reader[i][0] == 0) {
                    PORTA &= ~(1 << PA0); // LOW
                } else {
                    PORTA |= (1 << PA0);  // HIGH
                }
                if (Reader[i][1] == 0) {
                    PORTA &= ~(1 << PA2); // LOW
                } else {
                    PORTA |= (1 << PA2);  // HIGH
                }
                if (Reader[i][2] == 0) {
                    PORTA &= ~(1 << PA4); // LOW
                } else {
                    PORTA |= (1 << PA4);  // HIGH
                }
                line_values[(j * 8) + i] = analogRead(outputPIN[j]); //値の保存
                if (line_values[(j * 8) + i] > detection_border) { //trueとfalseのステータスに変換
                    line_stat_[(j * 8) + i] += 1; //仮ステータスに加算
                    if (line_stat_[(j * 8) + i] >= 2) { //反応回数が2回であれば最終ステータスを１にする
                        line_stat[(j * 8) + i] = 1;
                    }
                }
            }
        }
    }

    // for (int i = 0; i < 23; i++) {
    //     Serial.print(line_values[i]);
    //     Serial.print(" ");
    // }
    //TODO


    //グループ処理の開始センサー番号を決める
    byte startNUM = 99;
    for (int i = 23; i > 0; i--) {
        if (line_stat[i] == 0 && startNUM == 99) {
            startNUM = i + 1;
        }
    }

    //グループ分けの初期化
    total_x = 0;
    total_y = 0;
    int pack_NUM = 0; //グループの個数
    bool pack_NOW = 0; //グループ処理中ステータス
    for(int i = 0; i < 4; i++) {
        pack_x[i] = 0;
        pack_y[i] = 0;
    }

    //グループ分け
    for (int i = startNUM; i < startNUM + 24; i++) {
        byte pLine = i % 24; //処理中のセンサー
        if (line_stat[pLine] == 1) {
            myvector.get_cord(line_degs[pLine], line_r);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
            pack_NOW = 1;
        } else {
            if (pack_NOW == 1) { //グループの終点を検知
                pack_NOW = 0;
                pack_x[pack_NUM] = total_x;
                pack_y[pack_NUM] = total_y;
                pack_NUM += 1;
                total_x = 0;
                total_y = 0;
            }
        }
    }

    // Serial.print("/ ");
    // for(int i = 0; i < 4; i++) {
    //     Serial.print(pack_degs[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();
    //TODO


    if (pack_NUM == 0) { //検知していない
        line_type = 0;
        line_x = 999;
        line_y = 999;
    } else { //検知している
        if (pack_NUM == 1) { //１つ検知
            line_type = 1;
            line_x = pack_x[0];
            line_y = pack_y[0];
        } else if (pack_NUM == 2) { //２つ検知
            //間が小さいほうを特定
            //ベクトルの移動平均
            line_x = 0;
            line_y = 0;
            line_type = 1;
        } else if (pack_NUM == 3) { //TODO 未実装
            //２番が角の可能性あり
            //２つがA辺、残りＢ辺がほとんど
            //ラインに対する最短距離の位置角が９０度に近いものを選ぶ
        } else if (pack_NUM == 4) { //TODO　未実装
            //しばらくは実装しなくていい
            //ラインに対する最短距離の位置角が９０度に近いものを選ぶ
        }
    }
}

int LINE::get_value(byte lineNUM) {
    return line_values[lineNUM];
}

int LINE::get_azimuth() {
    return myvector.get_azimuth(line_x, line_y);
}

int LINE::get_magnitude() {
    int mag = 999;
    if (line_type != 0) {
        mag = myvector.get_magnitude(line_x, line_y);
    }
    return mag;
}

int LINE::get_x() {
    return line_x;
}

int LINE::get_y() {
    return line_y;
}

int LINE::get_type() {
    return line_type;
}