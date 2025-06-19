#include "line.h"
#include "AIP.h"

void LINE::setup(void) {
    pinMode(22, OUTPUT);
    pinMode(24, OUTPUT);
    pinMode(26, OUTPUT);
    pinMode(9, INPUT);
    pinMode(13, INPUT);
    pinMode(11, INPUT);
}

void LINE::read() {
    //リセット
    for (int i = 0; i < 4; i++) {
        pack_degs[i] = 0;
    }
    for (int i = 0; i < 24; i++) {
        line_stat_[i] = 0;
        line_stat[i] = 0;
    }

    //読み取り
    for (int k = 0; k < 2; k++) {
        for (int j = 0; j < 3; j++) { 
            for (int i = 0; i < 8; i++) {
                if (i == 0 || i == 1 || i == 2 || i == 3) {
                    digitalWrite(22, LOW);
                } else {
                    digitalWrite(22, HIGH);
                }
                if (i == 0 || i == 1 || i == 4 || i == 5) {
                    digitalWrite(24, LOW);
                } else {
                    digitalWrite(24, HIGH);
                }
                if (i == 0 || i == 2 || i == 4 || i == 6 || i == 8) {
                    digitalWrite(26, LOW);
                } else {
                    digitalWrite(26, HIGH);
                }
                // digitalWrite(22, HIGH);
                // digitalWrite(24, HIGH);
                // digitalWrite(26, HIGH);

                // line_values[(j * 8) + i] = analogRead(9);
                line_values[(j * 8) + i] = analogRead(outputPIN[j]);

                Serial.print(analogRead(outputPIN[j]));
                Serial.print(" / ");
                if (line_values[(j * 8) + i] > detection_border) {
                    line_stat_[(j * 8) + i] += 1;
                    if (line_stat_[(j * 8) + i] == 2) {
                        line_stat[(j * 8) + i] = 1;
                    }
                }
            }
        }
        Serial.println();
    }
    
    //グループ分けを始めるセンサー番号決定
    byte startNUM = 99;
    for (byte i = 23; i > 0; i--) {
        if (line_stat[i] == 0 && startNUM == 99) {
            startNUM = i + 1;
            break;
        }
    }

    bool pack_NOW = 0;
    int pack_NUM = 0;
    total_x = 0;
    total_y = 0;
    //グループ分けをし、グループごとの角度を求める
    for (byte i = startNUM; i < startNUM + 23; i++) {
        byte pLine = i % 24;
        if (line_stat[pLine] == 1) {
            myvector.get_cord(line_degs[pLine], line_r);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
            pack_NOW = 1;
        } else {
            if (pack_NOW == 1) {
                pack_NOW = 0;
                pack_degs[pack_NUM] = myvector.get_azimuth(total_x, total_y); 
                pack_NUM += 1;
                total_x = 0;
                total_y = 0;
            }
        }
    }

    
    if (pack_NUM != 0) { //検知しているしてるかを試す
        smallest = 0;
        smallest_pack = 999;
        for (byte i = 0; i < pack_NUM + 1; i++) {
            nerror[i] = pack_degs[i] % 90;
            if (nerror[i] > 45) {
                nerror[i] -= 90;
            }
            if (smallest_pack > nerror[i]) {
                smallest = i;
                smallest_pack = nerror[i];
            }
        }
        
        point1 = 999; 
        point2 = 999; 
        point3 = 999;   
        point4 = 999; 
        for (byte i = 0; i < pack_NUM + 1; i++) {
            if (packNUM = 2 && i != smallest) {//3つの時、点反応と思われるグループを除外する
                if (point1 == 999) {
                    point1 = pack_degs[i];
                } else if (point2 == 999) {
                    point2 = pack_degs[i];
                } else if (point3 == 999) {
                    point3 = pack_degs[i];
                } else if (point4 == 999) {
                    point4 = pack_degs[i];
                }
            }
        }
        //あとで.hで定義しまくる
        line_dif = (360 - point1 + point2) % 360;
        line2_dif = (360 - point3 + point4) % 360;
        line_deg = (point1 + (line_dif / 2)) % 360;
        line2_deg = (point3 + (line_dif / 2)) % 360;
        line_theta = line_dif / 2;
        line2_theta = line2_dif / 2;
        line_dist = degrees(cos(radians(line_theta * line_r)));
        line2_dist = degrees(cos(radians(line2_theta * line_r)));
        
        if (pack_NUM + 1 == 1) {
            line_azimuth = pack_degs[0];
            line_magnitude = line_r;
            line_type = 1;
        } else if (pack_NUM + 1 == 2) {
            line_azimuth = line_deg;
            line_magnitude = line_dist;
            line_type = 1;
        } else if (pack_NUM + 1 == 3) { 
            total_x = 0;
            total_y = 0;
            
            myvector.get_cord((line_deg, line_dist);  
            total_x += myvector.get_x();
            total_y += myvector.get_y();         
            myvector.get_cord(pack_degs[smallest], line_r);
            total_x += myvector.get_x();
            total_y += myvector.get_y();

            line_azimuth = myvector.get_azimuth(total_x, total_y);
            line_magnitude = myvector.get_magnitude(total_x, total_y);
        
            line_type = 2;
        } else if (pack_NUM + 1 == 4) {
            total_x = 0;
            total_y = 0;
            
            myvector.get_cord(line_deg, line_dist);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
            myvector.get_cord(line2_deg, line2_dist);
            total_x += myvector.get_x();
            total_y += myvector.get_y();

            line_azimuth = myvector.get_azimuth(total_x , total_y);
            line_magnitude = myvector.get_magnitude(total_x, total_y);  

            line_type = 2;
        } 
        if (avoid_azimuth == 999) {
            avoid_azimuth = (line_azimuth + 180) % 360;
        }
    } else {
        line_azimuth = 0;
        line_magnitude = 999;
        line_type = 0;
        avoid_azimuth = 999;
    }
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
