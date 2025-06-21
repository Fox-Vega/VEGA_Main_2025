#include "line.h"
#include "AIP.h"

void LINE::setup(void) {
    for (int i = 0; i < 3; i++) {
        pinMode(selectPIN[i], OUTPUT);
        pinMode(outputPIN[i], INPUT);
    }
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
                line_values[(j * 8) + i] = analogRead(outputPIN[j]);
                
                if (line_values[(j * 8) + i] > detection_border) {
                    line_stat_[(j * 8) + i] += 1;
                    if (line_stat_[(j * 8) + i] == 2) {
                        line_stat[(j * 8) + i] = 1;
                    }
                }
            }
        }
    }
    for (int i = 0; i < 24; i++) {
        Serial.print(line_stat[i]);
        Serial.print(" ");
    }
    
    //グループ分けを始めるセンサー番号決定
    byte startNUM = 99;
    for (int i = 23; i > 0; i--) {
        if (line_stat[i] == 0 && startNUM == 99) {
            startNUM = i + 1;
        }
    }
    Serial.print("/");
    Serial.print(startNUM);
    Serial.print(" ");


    bool pack_NOW = 0;
    int pack_NUM = 0;
    total_x = 0;
    total_y = 0;
    
    //グループ分けをし、グループごとの角度を求める
    for (int i = startNUM; i < startNUM + 24; i++) {
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
    
    for (int i = 0; i < 4; i++) {
        Serial.print(pack_degs[i]);
        Serial.print(" ");
    }

    
    if (pack_NUM != 0) { //検知しているしてるかを試す
        smallest = 999;
        smallest_pack = 99;
        if (pack_NUM == 3) {
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
        }

        point1_ = 999; 
        point2_ = 999; 
        point3_ = 999;   
        point4_ = 999; 
        for (byte i = 0; i < pack_NUM; i++) {
            if (i != smallest_pack) {
                if (point1_ == 999) {
                    point1_ = pack_degs[i];
                } else if (point2_ == 999) {
                    point2_ = pack_degs[i];
                } else if (point3_ == 999) {
                    point3_ = pack_degs[i];
                } else if (point4_ == 999) {
                    point4_ = pack_degs[i];
                }
            }
        }

        point1 = point1_;
        point2 = point2_;
        point3 = point3_;
        point4 = point4_;

        if ((360 - point1_ + point2_) % 360 > (360 - point2_ + point1_) % 360 && (pack_NUM == 2 || pack_NUM == 3)) { //2か3つ反応している場合、一つ目を補正するため。
            point1 = point2_;
            point2 = point1_;
        }
        if ((360 - point4_) % 360 < point4_ - point3_ && pack_NUM == 4) { //4つ反応している場合、1つめを補正するため
            point1 = point4_;
            point2 = point1_;
            point3 = point2_;
            point4 = point3_;
        }

        line_dif = (point2 - point1 + 360) % 360;
        line2_dif = (point4 - point3 + 360) % 360;
        line_deg = (point1 + line_dif / 2) % 360;
        line2_deg = (point3 + line2_dif / 2) % 360;
        line_theta = line_dif / 2;
        line2_theta = line2_dif / 2;
        line_dist = line_r * cos(radians(line_theta));
        line2_dist = line_r * cos(radians(line2_theta));
        
        if (pack_NUM == 1) {
            line_azimuth = pack_degs[0];
            line_magnitude = line_r;
            line_type = 1;
        } else if (pack_NUM == 2) {
            line_azimuth = line_deg;
            line_magnitude = line_dist;
            line_type = 1;
        } else if (pack_NUM == 3) { 

            total_x = 0;
            total_y = 0;
            
            myvector.get_cord(line_deg, line_dist);  
            total_x += myvector.get_x();
            total_y += myvector.get_y();         
            myvector.get_cord(pack_degs[smallest], line_r);
            total_x += myvector.get_x();
            total_y += myvector.get_y();

            line_azimuth = myvector.get_azimuth(total_x, total_y);
            line_magnitude = myvector.get_magnitude(total_x, total_y);
        
            line_type = 2;
        } else if (pack_NUM == 4) {
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
        if ((line_azimuth - avoid_azimuth + 360) % 360 < 90) {
            avoid_azimuth = line_azimuth;
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