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
    //リセット
    for(int i = 0; i < 4; i++) {
        pack_degs[i] = 0;
    }
    for (int i = 0; i < 24; i++) {
        line_stat_[i] = 0;
        line_stat[i] = 0;
    }

    //読み取り
    for (int k = 0; k < 2; k++) { //2回測定
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

            for (int j = 0; j < 3; j++) { //3つのマルチプレクサを読む
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

    //グループ処理の開始センサー番号を決める
    byte startNUM = 99;
    for (int i = 23; i > 0; i--) {
        if (line_stat[i] == 0 && startNUM == 99) {
            startNUM = i + 1;
        }
    }

    //グループ分け
    total_x = 0;
    total_y = 0;
    int pack_NUM = 0; //グループの個数
    bool pack_NOW = 0; //グループ処理中ステータス
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
                pack_degs[pack_NUM] = myvector.get_azimuth(total_x, total_y); 
                pack_NUM += 1;
                total_x = 0;
                total_y = 0;
            }
        }
    }


    if (pack_NUM != 0) { //検知しているしてるかを確認
        if (pack_NUM == 1) {
            myvector.get_cord(pack_degs[0], line_r);
            line_x = myvector.get_x();
            line_y= myvector.get_y();
            line_type = 1;
        } else if (pack_NUM == 2) {
            line_dif = (pack_degs[1] - pack_degs[0] + 360) % 360;
            if (line_dif > 180) {
                line_dif = (360 - line_dif + 360) % 360;
            }
            
            line_deg = (pack_degs[0] + line_dif / 2) % 360;
            line_theta = line_dif / 2;
            line_dist = line_r * cos(radians(line_theta));

            myvector.get_cord(line_deg, line_dist);
            line_x = myvector.get_x();
            line_y = myvector.get_y();
            line_type = 1;
        } else if (pack_NUM == 3) {
            total_x = 0;
            total_y = 0;

            myvector.get_cord(line_deg, line_dist);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
            myvector.get_cord(pack_degs[dot], line_r);
            total_x += myvector.get_x();
            total_y += myvector.get_y();

            line_x= total_x;
            line_y = total_y;

            line_type = 2;
        } else if (pack_NUM == 4) {
            byte first_line = 0;
            short max_dif = 0;
            
            for (byte i = 0; i < 4; i++) {
                byte pline = (i + 1) % 4;
                short dif = pack_degs[pline] - pack_degs[i];
                if (dif > max_dif) {
                    first_line = pline; //line1の1番センサーを決める
                    max_dif = dif;
                }
            }

            line_dif = (pack_degs[first_line + 1] - pack_degs[first_line] + 360) % 360;
            line2_dif = (pack_degs[first_line + 3] - pack_degs[first_line + 2] + 360) % 360;
            line_deg = (pack_degs[first_line] + line_dif / 2) % 360;
            line2_deg = (pack_degs[first_line + 2] + line2_dif / 2) % 360;
            line_theta = line_dif / 2;
            line2_theta = line2_dif / 2;
            line_dist = line_r * cos(radians(line_theta));
            line2_dist = line_r * cos(radians(line2_theta));

            total_x = 0;
            total_y = 0;
            myvector.get_cord(line_deg, line_dist);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
            myvector.get_cord(line2_deg, line2_dist);
            total_x += myvector.get_x();
            total_y += myvector.get_y();

            line_x = total_x;
            line_y = total_y;

            line_type = 2;
        }

        avoid_azimuth_ = (line_azimuth + 180) % 360;
        if (favoid_azimuth == 999) {
            favoid_azimuth = avoid_azimuth_;
        }

        line_dif = ((avoid_azimuth_ - favoid_azimuth) + 180) % 360 - 180;

        if (abs(line_dif) < 120) {
            avoid_azimuth = avoid_azimuth_;
        }

    } else {
        line_azimuth = 0;
        line_magnitude = 999;
        line_type = 0;
        avoid_azimuth = 999;
        favoid_azimuth = 999;
    }

    // Serial.print("/ ");
    // Serial.println(line_azimuth);
}

int LINE::get_value(byte lineNUM) {
    return line_values[lineNUM];
}

int LINE::get_azimuth() {
    return line_azimuth;
}

int LINE::get_avoid() {
    return avoid_azimuth;
}

int LINE::get_magnitude() {
    return line_magnitude;
}

int LINE::get_type() {
    return line_type;
}
