#include "line.h"

void LINE::setup(void) {
    pinMode(selectPIN[0], OUTPUT);
    pinMode(selectPIN[1], OUTPUT);
    pinMode(selectPIN[2], OUTPUT);
    pinMode(outputPIN[0], INPUT);
    pinMode(outputPIN[1], INPUT);
    pinMode(outputPIN[2], INPUT);
}

void LINE::read() {
    if (byte i = 0; i < 3; i++) {
        pack_degs[i] = 0;
    }
    for (int i = 0; i < 23; i++) {
        line_stat_[i] = 0;
        line_stat[i] = 0;
    }
    for (int k = 0; k < 2; k++) {
        for (int j = 0; j < 3; j++) { 
            for (int i = 0; i < 7; i++) {
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

                line_values[(j * 8) + i] = analogRead(j);
                if (line_values[(j * 8) + i] > detection_border) {
                    byte j_ = j;
                    byte i_ = i;
                    line_stat_[(j * 8) + i] += 1;
                    if (line_stat_[(j * 8) + i] == 2) {
                        line_stat[(j * 8) + i] = 1;
                    }
                }
            }
        }
    }
    int total_x = 0;
    int total_y = 0;
    bool pack_NOW = 0;
    byte pach_NUM = 0;

    byte startNUM = 99;
    for (byte i = 23; i > 0; i--) {
        if (line_stat[i] == 0 && startNUM == 99) {
            startNUM = i + 1;
            break;
        }
    }
    for (byte i = startNUM; i < startNUM + 23; i++) {
        byte pLine = i % 24;
        if (line_stat[pLine] == 1) {
            myvector.get_cord(line_degs[pLine], line_r);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
            if (pack_NOW == 0) {
                pack_NOW = 1;
            }
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
    if (pack_NUM != 0) {
        byte smallest = 0;
        byte smallest_pack = 999;
        for (byte i = 0; i < pack_NUM + 1; i++) {
            nerror[i] = pack_degs[i] % 90;
            if (nerror[i] > 45) {
                nerror[i] -= 90;
            }
            if (smallest_pack > nerror) {
                smallest = i;
                smallest_pack = nerror;
            }
        }
        int point1 = 999; 
        int point2 = 999; 
        for (byte i = 0; i < pack_NUM + 1; i++) {
            if (i != smallest && point1 == 999) {
                point1 = pack_degs[i];
            } else if (i != smallest && point2 == 999 && pack_degs[i] != point1) {
                point2 = pack_degs[i];
            }
        }
        int line_deg = (point1 + point2) / 2;
        int theta = line_deg - point1;
        line_dist = degrees(cos(radians(theta * line_r)));

        int total_x = 0;
        int total_y = 0;
        
        if (pack_NUM + 1 == 2) {
            line_azimuth = line_deg;
            line_magnitude = line_dist;
            line_type = 1;
        } else if (pack_NUM + 1 == 3) {
            myvector.get_cord(line_deg, line_dist);  
            total_x += myvector.get_x();
            total_y += myvector.get_y();         
            myvector.get_cord(pack_degs[smallest], line_r);
            total_x += myvector.get_x();
            total_y += myvector.get_y();

            line_azimuth = myvector.get_azimuth(total_x, total_y);
            line_magnitude = myvector.get_magnitude(total_x, total_y);
          
            line_type = 2;
        } else if (pack_NUM + 1 == 3) {
            int point3 = 999;   
            int point4 = 999; 
            for (byte i = 0; i < pack_NUM + 1; i++) {
                if (i != smallest && point3 == 999) {
                    point3 = pack_degs[i];
                } else if (i != smallest && point4 == 999 && pack_degs[i] != point3) {
                    point4 = pack_degs[i];
                }
            }
            int line2_deg = (point3 + point4) / 2;
            int theta = line2_deg - point3;
            line2_dist = degrees(cos(radians(theta * line_r)));
        
            myvector.get_cord(line_deg, line_dist);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
            myvector.get_cord(line_deg2, line_dist2);
            total_x += myvector.get_x();
            total_y += myvector.get_y();

            line_azimuth = myvector.get_azimuth(total_x , total_y);
            line_magnitude = myvector.get_magnitude(total_x, total_y);  

            line_type = 3;
        } 
    } else {
        line_azimuth = 0;
        line_magnitude = 999;
        line_type = 0;
    }
}

int LINE::get_value(byte lineNUM) {
    return line_values[lineNUM];
}

int LINE::get_azimuth() {
    return line_azimuth;
}

int LINE::get_magnitude() {
    return line_magnitude;
}

int LINE::get_type() {
    return line_type;
}
