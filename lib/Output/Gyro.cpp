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
    sensors_event_t gyro_event, mag_event;
    bno.getEvent(&gyro_event, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&mag_event, Adafruit_BNO055::VECTOR_MAGNETOMETER);

    float dt = (float)(millis()- old_azimuthtime);
    old_azimuthtime = millis();

    // **ジャイロデータから角度を積分**
    float gyroYawRate = gyro_event.gyro.z; // 角速度 [°/s]
    float gyroAzimuth = azimuth + gyroYawRate * dt; // 積分で推定

    // **磁気センサーから絶対方位を取得**
    float magAzimuth = degrees(atan2(mag_event.magnetic.y, mag_event.magnetic.x));
    if (magAzimuth < 0) magAzimuth += 360; // 0~360°に調整

    // **相補フィルタを適用**
    azimuth = alpha * gyroAzimuth + (1 - alpha) * magAzimuth;

    return (int)azimuth;
}

void Gyro::get_cord() {
    // BNO055から加速度データを取得（単位：m/s^2）
    for (int i = 0; i < 2; i++) {
        float dt = (millis() - old_cordtime) / 1000.0; // 秒単位に変換
        
        sensors_event_t event;
        sensors_event_t euler_event;
        
        bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);
        float a[3] = {event.acceleration.x, event.acceleration.y, event.acceleration.z };
        Serial.print(">Accel_x:");
        Serial.println(a[0]);
        Serial.print(">Accel_y:");
        Serial.println(a[1]);

        bno.getEvent(&euler_event, Adafruit_BNO055::VECTOR_EULER);
        yaw_rad = radians(euler_event.orientation.x + yawtweak); // Yawをラジアンに変換

        // バイアス補正
        for (int i = 0; i < 3; i++) {
            a[i] -= accel_bias[i];
        }

        // ノイズフィルタリング
        for (int i = 0; i < 3; i++) {
            if (fabs(a[i]) < accel_noise) { // 絶対値で判定
                a[i] = 0;
            }
        }

        // 衝突時の加速度を無効化
        if (fabs(a[0]) > collision_border || fabs(a[1]) > collision_border) {
            a[0] = a[1] = a[2] = 0.0;
        }

        // フィルター適用
        lowpassvalue_x = lowpassvalue_x * filterCoefficient + a[0] * (1 - filterCoefficient);
        lowpassvalue_y = lowpassvalue_y * filterCoefficient + a[1] * (1 - filterCoefficient);
        highpassvalue_x = a[0] - lowpassvalue_x;
        highpassvalue_y = a[1] - lowpassvalue_y;

        // 速度計算（修正）
        speed_x += ((highpassvalue_x + old_accel_x) * dt) / 2;
        speed_y += ((highpassvalue_y + old_accel_y) * dt) / 2;
        old_accel_x = highpassvalue_x;
        old_accel_y = highpassvalue_y;
        
        states[0] += speed_x * dt;
        states[1] += speed_y * dt;
        old_cordtime = millis();
    }

    world_x = states[0] * cos(-yaw_rad) - states[1] * sin(-yaw_rad);
    world_y = states[0] * sin(-yaw_rad) + states[1] * cos(-yaw_rad);

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
    return world_x;
}

int Gyro::get_y() {
    return world_y;
}