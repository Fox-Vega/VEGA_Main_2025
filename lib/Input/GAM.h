#pragma once

#include <Arduino.h>

class GAM { //いろいろエラーが出てたから弄った結果、GAMクラス誕生☆　Gyro,Accelometer,Magneticをテキトーにもじりました
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
        const float accel_noise = 0.25f; //動いていないと判断した際に使用するフィルタ（加速度センサーのノイズ除去）
        const float adaptive_noise = 0.02f; //動いていると判断した際に使用するフィルタ
        const float collision_border = 4.0f; //衝突判定ボーダー
        const float movement_border = 0.4f; //動作判定ボーダー

        int azimuth;
        // int world_x;
        // int world_y;
        int yawtweak;
        int zero_count;
        int ten_count;
        int j;
        int PoMi[2] = {1, 1}; //1は1~、0は~-1、10は0
        int first_PoMi[2] = {10, 10};
        bool move[2] = {false, false};
        bool collision;
        bool moving;
        float old_accel_data[2];
        float difcord_x;
        float difcord_y;
        float yaw_rad;
        float speed_x;
        float speed_y;
        float old_cordtime;
        float accel_bias[3] = {0.0, 0.0, 0.0};

        //自己位置推定用
        float states[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
};
