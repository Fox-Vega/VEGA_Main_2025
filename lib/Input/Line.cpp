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
    for (int k = 0; k < 1; k++) { //1回測定
        for (int j = 0; j < 3; j++) { //3つのマルチプレクサを読む
            for (int i = 0; i < 8; i++) { //8回の変更
                if (Reader[i][0] == 0) {
                    digitalWrite(selectPIN[0], LOW);
                } else {
                    digitalWrite(selectPIN[0], HIGH);
                }
                if (Reader[i][1] == 0) {
                    digitalWrite(selectPIN[1], LOW);
                } else {
                    digitalWrite(selectPIN[1], HIGH);
                }
                if (Reader[i][2] == 0) {
                    digitalWrite(selectPIN[2], LOW);
                } else {
                    digitalWrite(selectPIN[2], HIGH);
                }
                line_values[(j * 8) + i] = analogRead(outputPIN[j]); //値の保存
                if (line_values[(j * 8) + i] > detection_border) { //trueとfalseのステータスに変換
                    line_stat_[(j * 8) + i] += 1; //仮ステータスに加算
                    if (line_stat_[(j * 8) + i] >= 1) { //反応回数が2回以上であれば最終ステータスを１にする
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
        if (line_type = 0) {
            trip = true;
        } else {
            trip = false;
        }
        if (pack_NUM == 1) { //１つ検知
            line_type = 1;
            line_x = pack_x[0];
            line_y = pack_y[0];
            trip = false;
            over = false;
        } else if (pack_NUM == 2) { //２つ検知
            line_type = 1;
            line_x = (pack_x[0] + pack_x[1]) / 2;
            line_y = (pack_y[0] + pack_y[1]) / 2;
        } else if (pack_NUM == 3) { //TODO 未実装
            line_type = 2;
            
            float dif = 360
            int close = 0
            
            float aside_x = (pack_x[0] + pack_x[1]) / 2;
            float aside_y = (pack_y[0] + pack_y[1]) / 2;
            float pata = myvector.get_vectordegrees(aside_x, aside_y, pack_x[2], pack[2]);
            if (pata < dif) {
                close = 1;
                dif = pata;
            }
            
            float bside_x = (pack_x[1] + pack_x[2]) / 2;
            float bside_y = (pack_y[1] + pack_y[2]) / 2;
            float patb = myvector.get_vectordegrees(bside_x, bside_y, pack_x[0], pack[0]);
            if (patb < dif) {
                close = 2;
                dif = patb;
            } 
            
            float cside_x = (pack_x[0] + pack_x[2]) / 2;
            float cside_y = (pack_y[0] + pack_y[2]) / 2;
            float patc = myvector.get_vectordegrees(cside_x, cside_y, pack_x[1], pack[1]);
            if (patc < dif) {
                close = 3;
                dif = patc;
            }
            
            if (close = 1) {
                line_x = aside_x + pack_x[2];
                line_y = aside_y + pack_y[2];
            } else if (close = 2) {
                line_x = bside_x + pack_x[0];
                line_y = bside_y + pack_y[0];
            } else {
                line_x = cside_x + pack_x[1];
                line_y = cside_y + pack_y[1];
            }
        } else if (pack_NUM == 4) { //TODO　未実装
            //しばらくは実装しなくていい
            //ラインに対する最短距離の位置角が９０度に近いものを選ぶ
        }
        if (myvector.get_vectordegrees(line_x, line_y, oldline_x, oldline_y) > 120 && trip == false) {
            if (over == false) over = true;
            if (over == true) over = false;
        }
        if (over == false) {
            escape_x = -line_x;
            escape_ｙ = -line_ｙ;
        ｝
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
