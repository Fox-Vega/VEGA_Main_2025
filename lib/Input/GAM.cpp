#include "GAM.h"
#include "Input.h"
#include "Output.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void GAM::setup() {
    mypixel.multi(0, 15, 225, 0, 0);
    Wire.begin();
    if (!bno.begin()) {
        Serial.println("BNO055 not detected.");
        while (1);  //センサー未検出時は停止
    }
    bno.setExtCrystalUse(true);
    bno.setMode(OPERATION_MODE_AMG);
    delay(1000);
    azimuth = 0;
    while (millis() < 4000) {
        sensors_event_t accel_event;
        bno.getEvent(&accel_event, Adafruit_BNO055::VECTOR_ACCELEROMETER);  
        float accel_data[2] = {accel_event.acceleration.x, accel_event.acceleration.y};
        for (int i = 0; i < 2; i++) {
            accel_bias[i] = (accel_bias[i] + accel_data[i]) * 0.5; //平均値を計算
        }
    }
    mypixel.clear();
}

int GAM::get_azimuth() {
    sensors_event_t euler_event;
    bno.getEvent(&euler_event, Adafruit_BNO055::VECTOR_EULER);

    return (int)(euler_event.orientation.x);
}

void GAM::get_cord() {
    //BNO055から加速度データを取得（単位：m/s^2）
    float dt = (millis() - old_cordtime) / 1000.0; //秒単位に変換

    sensors_event_t event;
    bno.getEvent(&event, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    float accel_data[2] = {(event.acceleration.x - accel_bias[0]) * accel_offset_x, (event.acceleration.y - accel_bias[1]) * accel_offset_y};

    for (int i = 0; i < 2; i++) { //処理軸以外が移動を検知していた場合、ノイズの判定を緩くする（加速度センサーの性質を利用）
        if (i == 0) {
            j = 1;
        } else {
            j = 0;
        }
        if (accel_data[j] > accel_noise) {
            if (fabs(accel_data[i]) < adaptive_noise) {
                accel_data[i] = 0;
            }
        } else {
            if (fabs(accel_data[i]) < accel_noise) {
                accel_data[i] = 0;
            }
        }
        if ((oold_accel_data[i] - accel_data[i]) == 0) {
            accel_data[i] = 0;
            if (accel_data[i] > old_accel_data[i]) {
                accel_bias[i] += (accel_data[i] / 2);
            } else {
                accel_bias[i] += (old_accel_data[i] / 2);
            }
        }
        float a = accel_tweaker / fabs(accel_data[i]);
        if (accel_data[i] != 0.0f) {
            if (accel_data[i] > 0) {
                accel_data[i] += a;
            } else {
                accel_data[i] -= a;
            }
        }
    }

    //値の大小で移動方向を判断するだけでなく、前回との差を考慮して移動しているかを判定する。
    for (int i = 0; i < 2; i++) { 
        float accel_dif = old_accel_data[i] - accel_data[i];
        if(fabs(accel_dif) < movement_border) {
            ten_count += 1;
            if (ten_count >= reset_border) {
                first_PoMi[i] = 10;
                PoMi[i] = 10;
                speed[0] = 0;
                speed[1] = 0;
                accel_data[0] = 0;
                accel_data[1] = 0;
            }
        } else if(accel_data[i] > 0) {
            ten_count = 0;
            if (first_PoMi[i] == 10) { //直前まで静止していたら初回動作検知方向に現在の方向を記録
                first_PoMi[i] = 1;
            }
            PoMi[i] = 1;
        } else { //-方向に動いている時の処理
            ten_count = 0;
            if (first_PoMi[i] == 10) {
                first_PoMi[i] = 0;
            }
            PoMi[i] = 0;
        }
        if (first_PoMi[i] != PoMi[i]) { //初回動作検知方向と現在の動きが異なる場合は0の位置を求めて速度計算
            a = fabs(old_accel_data[i]);
            b = fabs(accel_data[i]);
            if (a == 0 ||  b == 0) {
                a_dt = 0.0;
                b_dt = 0.0;
            } else {
                a_dt = dt * (a / (a + b));
                b_dt = dt * (b / (a + b));
            }
            Serial.print(">a_dt:");
            Serial.println(a_dt);
            Serial.print(">b_dt:");
            Serial.println(b_dt);
            Serial.print(">a:");
            Serial.println(a);
            Serial.print(">b:");
            Serial.println(b);
            gam.get_speed(a_dt, 0, i);
            gam.get_speed(b_dt, accel_data[i], i);
        } else {
            gam.get_speed(dt, accel_data[i], i);
        }
    }
    states[0] += speed[0] * 100 * dt;
    states[1] += speed[1] * 100 * dt;
    float yaw_rad = radians(gam.get_azimuth());
    world_x += states[0] * cos(yaw_rad) - states[1] * sin(yaw_rad);
    world_y += states[0] * sin(yaw_rad) + states[1] * cos(yaw_rad);
    
    //最終情報更新
    old_cordtime = millis();
    oold_accel_data[0] = old_accel_data[0];
    oold_accel_data[1] = old_accel_data[1];
    old_accel_data[0] = accel_data[0];
    old_accel_data[1] = accel_data[1];

    Serial.print(">Accel_x:");
    Serial.println(accel_data[0]);
    Serial.print(">Accel_y:");
    Serial.println(accel_data[1]);
    Serial.print(">pos_x:");
    Serial.println(states[0]);
    Serial.print(">pos_y:");
    Serial.println(states[1]);
    Serial.print(">Azimuth:");
    Serial.println(gam.get_azimuth());
    Serial.print(">DT:");
    Serial.println(dt);
}

void GAM::get_speed(float dt, float accel,short i) {
    // https://qiita.com/mzk1644/items/ea621cc872acd996a6e8 この記事のコードを使わせていただきました
    //
    lowpassValue[i] = lowpassValue[i] * filterCoefficient + accel * (1 - filterCoefficient);
    highpassValue[i] = accel - lowpassValue[i];
    speed[i] = (float)((highpassValue[i] + old_accel_data[i]) * dt) / 2 + speed[i];
    old_accel_data[i] = highpassValue[i];
    Serial.print(">Speed_x:");
    Serial.println(speed[0]);
    Serial.print(">Speed_y:");
    Serial.println(speed[1]);
}

void GAM::dir_reset() {
    yawtweak = gam.get_azimuth();
}

void GAM::cord_reset() {
    states[0] = 0;
    states[1] = 0;
}

void GAM::restart() { //瞬間的にモードを変えることで初期化
    bno.setMode(OPERATION_MODE_CONFIG);
    delay(25);
    bno.setMode(OPERATION_MODE_AMG);
    delay(1000);
    while (millis() < 5000) {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);  
        float accel_data[3] = { event.acceleration.x, event.acceleration.y, event.acceleration.z };
        for (int i = 0; i < 3; i++) {
            accel_bias[i] = (accel_bias[i] + accel_data[i]) * 0.5; //平均値を計算
        }
    }
}

int GAM::get_x() {
    return states[0];
}

int GAM::get_y() {
    return states[1];
}