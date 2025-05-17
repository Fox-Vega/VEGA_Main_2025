#include "GAM.h"
#include "Input.h"
#include "Output.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void GAM::setup() {
    mypixel.multi(1, 16, 225, 0, 0);
    Wire.begin();
    if (!bno.begin()) {
        Serial.println("BNO055 not detected.");
        while (1);  //センサー未検出時は停止
    }
    bno.setExtCrystalUse(true);
    bno.setMode(OPERATION_MODE_AMG);
    delay(1000);
    azimuth = 0;
    while (millis() < 3000) {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);  
        float accel_data[2] = {event.acceleration.x, event.acceleration.y};
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
    bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);
    float accel_data[2] = {event.acceleration.x - accel_bias[0], event.acceleration.y - accel_bias[1]};

    Serial.print(">Accel_x:");
    Serial.println(accel_data[0]);
    Serial.print(">Accel_y:");
    Serial.println(accel_data[1]);

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
    }

    //値の大小で移動方向を判断するだけでなく、前回との差を考慮して移動しているかを判定する。
    for (int i = 0; i < 2; i++) { 
        if((fabs(old_accel_data[i] - accel_data[i])) > movement_border) {
            ten_count = 0;
            zero_count = 0;
            if (accel_data[i] > 0.2f) { //+方向に動いている時の処理
                if (first_PoMi[i] == 10) { //直前まで静止していたら初回動作検知方向に現在の方向を記録
                    first_PoMi[i] = 1;
                }
                PoMi[i] = 1;
            } else if (accel_data[i] < -0.2f) { //-方向に動いている時の処理
                if (first_PoMi[i] == 10) {
                    first_PoMi[i] = 0;
                }
                PoMi[i] = 0;
            }
        } else {
            ten_count += 1;
            if (ten_count >= 2) {
                first_PoMi[i] = 10;
                PoMi[i] = 10;
            }
            if (zero_count <= 2) {
                zero_count += 1;
            } else {
                speed_x = 0;
                speed_y = 0;
                accel_data[0] = 0;
                accel_data[1] = 0;
            }
        }

        if (first_PoMi[i] != PoMi[i]) { //初回動作検知方向と現在の動きが異なる場合は現在の動きをキャンセル（減速時の加速度を無効化）
            accel_data[i] = 0;
        }
    }

    //座標加算
    states[0] += accel_data[0] * dt * 100;
    states[1] += accel_data[1] * dt * 100;

    // states[0] = accel_data[0] * dt * 100;
    // states[1] = accel_data[1] * dt * 100;
    // float yaw_rad = radians(gyro.get_azimuth());
    // world_x += states[0] * cos(yaw_rad) - states[1] * sin(yaw_rad);
    // world_y += states[0] * sin(yaw_rad) + states[1] * cos(yaw_rad);
    
    //最終情報更新
    old_cordtime = millis();
    old_accel_data[0] = accel_data[0];
    old_accel_data[1] = accel_data[1];


    //TelePlot用
    speed_x += difcord_x / dt;
    speed_y += difcord_y / dt;
    difcord_x = accel_data[0] * dt * 100;
    difcord_y = accel_data[1] * dt * 100;

    Serial.print(">Azimuth:");
    Serial.println(gam.get_azimuth());
    Serial.print(">PoMi_x:");
    Serial.println(first_PoMi[0]);
    Serial.print(">PoMi_y:");
    Serial.println(first_PoMi[1]);
    Serial.print(">Zero Count:");
    Serial.println(zero_count);
    Serial.print(">Speed_x:");
    Serial.println(speed_x);
    Serial.print(">Speed_y:");
    Serial.println(speed_y);
    Serial.print(">DT:");
    Serial.println(dt);
    Serial.print(">Moving:");
    Serial.print(moving);
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

void GAM::dir_reset() {
    yawtweak = gam.get_azimuth();
}
int GAM::get_x() {
    return states[0];
}

int GAM::get_y() {
    return states[1];
}