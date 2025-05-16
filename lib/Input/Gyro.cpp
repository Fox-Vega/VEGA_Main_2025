#include "Gyro.h"
#include "Input.h"

void Gyro::setup() {
    Wire.begin();
    if (!bno.begin()) {
        Serial.println("BNO055 not detected.");
        while (1);  //センサー未検出時は停止
    }
    bno.setExtCrystalUse(true);
    bno.setMode(OPERATION_MODE_AMG);
    delay(1000);
    azimuth = 0;
    while (millis() < 5000) {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);  
        float a[3] = { event.acceleration.x, event.acceleration.y, event.acceleration.z };
        for (int i = 0; i < 3; i++) {
            accel_bias[i] = (accel_bias[i] + a[i]) * 0.5; //平均値を計算
        }
    }
}

int Gyro::get_azimuth() {
    sensors_event_t euler_event;
    
    //Euler角（heading, roll, pitch）を取得
    bno.getEvent(&euler_event, Adafruit_BNO055::VECTOR_EULER);
    
    //取得したEuler角のX成分がheading（単位: 度）です
    float heading = euler_event.orientation.x;
    return (int)heading;
}

void Gyro::get_cord() {
    //BNO055から加速度データを取得（単位：m/s^2）
    float dt = (millis() - old_cordtime) / 1000.0; //秒単位に変換
    yaw_rad = gyro.get_azimuth();
    sensors_event_t event;
    bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);
    float a[3] = {event.acceleration.x - accel_bias[0], event.acceleration.y - accel_bias[1], event.acceleration.z - accel_bias[2]};
    int azimuth = gyro.get_azimuth();

    Serial.print(">Accel_x:");
    Serial.println(a[0]);
    Serial.print(">Accel_y:");
    Serial.println(a[1]);

    first_PoMi[1] = first_PoMi[0];
    first_PoMi[2] = first_PoMi[1];
    first_PoMi[4] = first_PoMi[3];
    first_PoMi[5] = first_PoMi[4];

    for (int i = 0; i < 2; i++) {
        if (i = 0) {
            j = 0;
        } else {
            j = 2;
        }
        if (fabs(a[i]) < accel_noise) { //ノイズを除外
            a[i] = 0;
        }
        if (a[i] > 0) {
            if (first_PoMi[j] == 10) {
                first_PoMi[j] = 1;
            }
            PoMi[i] = 1;
        } else if (a[i] == 0) {
            first_PoMi[i + j] = 10;
            PoMi[i] = 10;
        } else {
            if (first_PoMi[j] == 10) {
                first_PoMi[j] = 0;
            }
            PoMi[i] = 0;
        }
        if (first_PoMi[j] != PoMi[i]) {
            a[i] = 0;
        }
    }
    Serial.print(">Azimuth:");
    Serial.println(azimuth);
    Serial.print(">PoMi_x:");
    Serial.println(first_PoMi[1]);
    Serial.print(">PoMi_y:");
    Serial.println(first_PoMi[3]);


    if (a[0] == 0.0f) { //停止検知
        zero_count_x += 1;
    } else {
        zero_count_x = 0;
    }
    if (zero_count_x >= 3) {
        speed_x = 0;
        speed_y = 0;
        a[0] = 0;
        a[1] = 0;
    }
    if (a[1] == 0.0f) { //停止検知
        zero_count_y += 1;
    } else {
        zero_count_y = 0;
    }
    if (zero_count_y >= 3) {
        speed_x = 0;
        speed_y = 0;
        a[0] = 0;
        a[1] = 0;
    }

    Serial.print(">Zero Count_x:");
    Serial.println(zero_count_x);
    Serial.print(">Zero Count_y:");
    Serial.println(zero_count_y);

    if (fabs(a[0]) > collision_border || fabs(a[1]) > collision_border) { //衝突時の加速度を無効化
        a[0] = a[1] = a[2] = 0.0;
    }

    difcord_x = a[0] * dt * 100;
    difcord_y = a[1] * dt * 100;

    speed_x += difcord_x / dt;
    speed_y += difcord_y / dt;

    states[0] += difcord_x;
    states[1] += difcord_y;

    // world_x = states[0] * cos(yaw_rad) - states[1] * sin(yaw_rad);
    // world_y = states[0] * sin(yaw_rad) + states[1] * cos(yaw_rad);

    Serial.print(">Speed_x:");
    Serial.println(speed_x);
    Serial.print(">Speed_y:");
    Serial.println(speed_y);
    Serial.print(">DT:");
    Serial.println(dt);
    old_cordtime = millis();
}

void Gyro::restart() { //瞬間的にモードを変えることで初期化
    bno.setMode(OPERATION_MODE_CONFIG);
    delay(25);
    bno.setMode(OPERATION_MODE_AMG);
    delay(1000);
    while (millis() < 5000) {
        sensors_event_t event;
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);  
        float a[3] = { event.acceleration.x, event.acceleration.y, event.acceleration.z };
        for (int i = 0; i < 3; i++) {
            accel_bias[i] = (accel_bias[i] + a[i]) * 0.5; //平均値を計算
        }
    }
}

void Gyro::dir_reset() {
    yawtweak = gyro.get_azimuth();
}
int Gyro::get_x() {
    return states[0];
}

int Gyro::get_y() {
    return states[1];
}