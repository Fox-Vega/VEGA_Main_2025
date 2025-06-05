#include "GAM.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void GAM::setup() {
    Wire.begin();
    if (!bno.begin()) {
        while (1); // センサー未検出時は停止
    }
    bno.setExtCrystalUse(true);
    bno.setMode(OPERATION_MODE_AMG);
    delay(2000);
    mybuzzer.start(200, 200);
    delay(50);
    mybuzzer.start(200, 200);
    azimuth = 0;
    gam.cord_custom(0, 0);
    int sampleNUM[2] = {0, 0};
    float total_noise[2] = {0.0f, 0.0f};
    float a = millis();
    while (millis() - a < 1000) {
        sensors_event_t accel_event;
        bno.getEvent(&accel_event, Adafruit_BNO055::VECTOR_ACCELEROMETER);  
        float accel_data[2] = {accel_event.acceleration.x, accel_event.acceleration.y};
        for (int i = 0; i < 2; i++) {
            total_noise[i] += accel_data[i];
            sampleNUM[i]++;
        }
    }
    accel_bias[0] = total_noise[0] / sampleNUM[0]; // 平均値を計算
    accel_bias[1] = total_noise[1] / sampleNUM[1]; // 平均値を計算
}


int GAM::get_azimuth() {
    sensors_event_t euler_event;
    bno.getEvent(&euler_event, Adafruit_BNO055::VECTOR_EULER);
    azimuth = euler_event.orientation.x - yawtweak;
    if (azimuth < 0) {
        azimuth += 360;
    }
    return azimuth;
}

void GAM::get_cord() {
    float dt = millis() - old_cordtime; // 秒単位に変換

    sensors_event_t event;
    bno.getEvent(&event, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    float accel_data[2] = {event.acceleration.x - accel_bias[0], event.acceleration.y - accel_bias[1]};

    for (int i = 0; i < 2; i++) {  
        if (accel_data[i] > 0) { //倍率調整
            accel_data[i] *= accel_offsetp[robotNUM][i];
        } else if (accel_data[i] < 0) {
            accel_data[i] *= accel_offsetm[robotNUM][i];
        }
        
        int j = (i == 0) ? 1 : 0;
        if (accel_data[j] > accel_noise) { //他軸が動いているなら
            if (fabs(accel_data[i] - old_accel_data[i]) < adaptive_noise) {
                accel_data[i] = 0;
                ten_count++;
            }
            if (accel_data[i] > accel_sparknoise) { //スパークノイズをフィルタ
                accel_data[i] = 0;
                ten_count++;
            }
        } else if (fabs(accel_data[i] - old_accel_data[i]) < accel_noise) { //ノイズ判定なら０にする
            accel_data[i] = 0;
            ten_count++;
            if (accel_data[i] > accel_sparknoise) { //スパークノイズをフィルタ
                accel_data[i] = 0;
            }
        }
        if (fabs(accel_data[i]) > 0) {
            ten_count = 0;
        }
        if (ten_count >= reset_border) {
            first_PoMi[i] = 10;
            PoMi[i] = 10;
            speed[0] = 0;
            speed[1] = 0;
            accel_data[0] = 0;
            accel_data[1] = 0;
        }

        lowpassValue[i] = lowpassValue[i] * filterCoefficient + accel_data[i] * (1 - filterCoefficient);
        highpassValue[i] = accel_data[i] - lowpassValue[i];
        speed[i] += accel_data[i] * dt;
    }

    // 台形積分で座標算出　座標軸固定無し（機体基準）
    states[0] += ((speed[0] + old_speed[0]) * dt) / 2 * 100;
    states[1] += ((speed[1] + old_speed[1]) * dt) / 2 * 100;

    // 台形積分で座標算出　座標軸固定有（コート基準）
    int azimuth_y = 0 - gam.get_azimuth();
    if (azimuth_y < 0) {azimuth_y += 360;}
    int azimuth_x = (azimuth_y + 90) % 360;
    myvector.get_cord(azimuth_x, ((speed[0] + old_speed[0]) * dt) / 2 * 100);
    world_x += get_x();
    world_y += get_y();
    myvector.get_cord(azimuth_y, ((speed[1] + old_speed[1]) * dt) / 2 * 100);
    world_x += get_x();
    world_y += get_y();

    // 最終情報更新
    old_cordtime = millis();
    old_speed[0] = speed[0];
    old_speed[1] = speed[1];
    old_accel_data[0] = accel_data[0];
    old_accel_data[1] = accel_data[1];
}

void GAM::dir_reset() {
    sensors_event_t euler_event;
    bno.getEvent(&euler_event, Adafruit_BNO055::VECTOR_EULER);
    yawtweak = euler_event.orientation.x;
}

void GAM::cord_custom(int x, int y) {
    world_x = x;
    world_y = y;
    speed[0] = 0.0;
    speed[1] = 0.0;
    old_cordtime = millis();
}

void GAM::restart() { //瞬間的にモードを変えることで初期化
    int s = millis();
    bno.setMode(OPERATION_MODE_CONFIG);
    delay(25);
    bno.setMode(OPERATION_MODE_AMG);
    delay(1000);
    while ((millis() - s) < 1000) {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_ACCELEROMETER);  
        float accel_data[3] = { event.acceleration.x, event.acceleration.y};
        for (int i = 0; i < 2; i++) {
            accel_bias[i] = (accel_bias[i] + accel_data[i]) * 0.5; //平均値を計算
        }
    }
}

int GAM::get_x() {
    return (int)states[0];
}

int GAM::get_y() {
    return (int)states[1];
}