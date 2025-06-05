#include "MadgwickAHRS.h"
#include "GAM.h"
#include "Input.h"
#include "Output.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Madgwick filter;

void GAM::setup() {
    Wire.begin();
    if (!bno.begin()) {
        while (1); //センサー未検出時は停止
    }
    bno.setExtCrystalUse(true);
    bno.setMode(OPERATION_MODE_AMG);
    filter.begin(100);  //サンプリングレートを設定
    delay(2000);

    mybuzzer.start(200, 200);
    delay(50);
    mybuzzer.start(200, 200);

    azimuth = 0;
    gam.cord_custom(0, 0);
}

//void GAM::update_quaternion() {
//    sensors_event_t gyro, accel, mag;
//    bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);
//    bno.getEvent(&accel, Adafruit_BNO055::VECTOR_ACCELEROMETER);

//    filter.updateIMU(gyro.gyro.x, gyro.gyro.y, gyro.gyro.z, accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
//}

void GAM::update_quaternion() {
    sensors_event_t gyro, accel, mag;
    bno.getEvent(&gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&accel, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno.getEvent(&mag, Adafruit_BNO055::VECTOR_MAGNETOMETER);

    filter.update(gyro.gyro.x, gyro.gyro.y, gyro.gyro.z, accel.acceleration.x, accel.acceleration.y, accel.acceleration.z, mag.magnetic.x, mag.magnetic.y, mag.magnetic.z);
}

int GAM::get_azimuth() {
    update_quaternion();
    float yaw = atan2(2.0f * (filter.getQ1() * filter.getQ2() + filter.getQ0() * filter.getQ3()),
                      1.0f - 2.0f * (filter.getQ2() * filter.getQ2() + filter.getQ3() * filter.getQ3())) * 180.0f / M_PI;
    return (yaw < 0) ? yaw + 360 : yaw;
}

void GAM::get_cord() {
    float q0 = filter.getQ0();
    float q1 = filter.getQ1();
    float q2 = filter.getQ2();
    float q3 = filter.getQ3();

    sensors_event_t event;
    bno.getEvent(&event, Adafruit_BNO055::VECTOR_ACCELEROMETER);

    float accelX = event.acceleration.x;
    float accelY = event.acceleration.y;

    //座標変換 (クオータニオンベース)
    float worldAccelX = (1 - 2 * (q2 * q2 + q3 * q3)) * accelX + (2 * (q1 * q2 - q0 * q3)) * accelY;
    float worldAccelY = (2 * (q1 * q3 + q0 * q2)) * accelX + (1 - 2 * (q1 * q1 + q2 * q2)) * accelY;

    float dt = millis() - old_cordtime;

    //台形積分による速度更新
    speed[0] += ((old_accel_data[0] + worldAccelX) / 2.0) * dt;
    speed[1] += ((old_accel_data[1] + worldAccelY) / 2.0) * dt;

    //台形積分による座標計算
    states[0] += ((speed[0] + old_speed[0]) / 2.0) * dt * 100;
    states[1] += ((speed[1] + old_speed[1]) / 2.0) * dt * 100;

    //最終情報更新
    old_cordtime = millis();
    old_speed[0] = speed[0];
    old_speed[1] = speed[1];

    old_accel_data[0] = worldAccelX;
    old_accel_data[1] = worldAccelY;
}

void GAM::dir_reset() {
    filter.begin(100); //フィルタをリセット
    azimuth = 0;
}

void GAM::cord_custom(int x, int y) {
    world_x = x;
    world_y = y;
    speed[0] = 0.0;
    speed[1] = 0.0;
    old_cordtime = millis();
}

int GAM::get_x() {
    return (int)states[0];
}

int GAM::get_y() {
    return (int)states[1];
}