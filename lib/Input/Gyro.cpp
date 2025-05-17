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
        float a[2] = { event.acceleration.x, event.acceleration.y};
        for (int i = 0; i < 2; i++) {
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
    yaw_rad = radians(gyro.get_azimuth());
    sensors_event_t event;
    bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);
    float a[3] = {event.acceleration.x - accel_bias[0], event.acceleration.y - accel_bias[1], event.acceleration.z - accel_bias[2]};

    if (fabs(a[0]) > collision_border || fabs(a[1]) > collision_border) { //衝突時の加速度を無効化
        a[0] = a[1] = a[2] = 0.0;
    }

    Serial.print(">Accel_x:");
    Serial.println(a[0]);
    Serial.print(">Accel_y:");
    Serial.println(a[1]);

    for (int i = 0; i < 2; i++) { //処理軸以外が移動を検知していた場合、ノイズの判定を緩くする（加速度センサーの性質を利用）
        if (i == 0) {
            j = 1;
        } else {
            j = 0;
        }
        if (a[j] > accel_noise) {
            if (fabs(a[i]) < adaptive_noise) {
                a[i] = 0;
            }
        } else {
            if (fabs(a[i]) < accel_noise) {
                a[i] = 0;
            }
        }
    }
    
    for (int i = 0; i < 2; i++) {
        float dif_a = fabs(old_a[i] - a[i]);
        if (a[i] > 0.2f) { //（静止　・　＋方向に移動中　・　ー方向に移動中）　を記録
            if (dif_a > movement_border) {
                if (first_PoMi[i] == 10) {
                    first_PoMi[i] = 1;
                }
                PoMi[i] = 1;
            } else {
                first_PoMi[i] = 10;
                PoMi[i] = 10;
            }
        } else if (a[i] < -0.2f) {
            if (dif_a > movement_border) {
                if (first_PoMi[i] == 10) {
                    first_PoMi[i] = 0;
                }
                PoMi[i] = 0;
            } else {
                first_PoMi[i] = 10;
                PoMi[i] = 10;
            }
        } else {
            if (dif_a < movement_border) {
                first_PoMi[i] = 10;
                PoMi[i] = 10;
            }
        } //初めに動きを記録した向きと現在の動きが異なる場合は現在の動きをキャンセル（減速時の加速度を無効化）
        if (first_PoMi[i] != PoMi[i]) {
            a[i] = 0;
        }
    }

    if (a[0] == 0.0f && a[1] == 0.0f) { //静止検知数を記録・静止時処理
        zero_count += 1;
    } else {
        zero_count = 0;
    }
    if (zero_count >= 2) {
        speed_x = 0;
        speed_y = 0;
        a[0] = 0;
        a[1] = 0;
    }

    //変化量計算
    difcord_x = a[0] * dt * 100;
    difcord_y = a[1] * dt * 100;

     //速度計算
    speed_x += difcord_x / dt;
    speed_y += difcord_y / dt;

    //座標加算
    states[0] += difcord_x;
    states[1] += difcord_y;

    // world_x = states[0] * cos(yaw_rad) - states[1] * sin(yaw_rad);
    // world_y = states[0] * sin(yaw_rad) + states[1] * cos(yaw_rad);

    old_cordtime = millis();
    old_a[0] = a[0];
    old_a[1] = a[1];

    //TelePlot用
    Serial.print(">Azimuth:");
    Serial.println(gyro.get_azimuth());
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