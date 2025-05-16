#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class Gyro {
    public:
        void setup(); //初期設定
        int get_azimuth(); //方位角取得（ドリフト対策無）
        void get_cord(); //自己位置取得
        void dir_reset(); //方向初期化
        void cord_reset(); //位置情報初期化
        void cord_custom(int x, int y); //自由位置に座標を設定可能
        void restart(); //センサーを初期化
        int get_x(); //座標のxを取得
        int get_y(); //座標のyを取得
        float vector_norm(const float v[3]) {
            return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
        }

    private:
        //調整用
        const float accel_noise = 0.2f;
        const float collision_border = 8.0f;
        
        int azimuth;
        // int world_x;
        // int world_y;
        int yawtweak;
        int zero_count_x;
        int zero_count_y;
        int j;
        int PoMi[2] = {1, 1}; //1は1~、0は~-1、10は0
        // int first_PoMi[2] = {1, 1}; //TODO 後で消す
        int first_PoMi[4] = {10, 10, 10, 10};
        float difcord_x;
        float difcord_y;
        float yaw_rad;
        float speed_x;
        float speed_y;
        float old_cordtime;
        float accel_bias[3] = {0.0, 0.0, 0.0};
        Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

        //自己位置推定用
        float states[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
};
