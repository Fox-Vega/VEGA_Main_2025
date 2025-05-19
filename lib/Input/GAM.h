#pragma once

#include <Arduino.h>

class GAM { //いろいろエラーが出てたから弄った結果、GAMクラス誕生☆　Gyro,Accelometer,Magneticをテキトーにもじりました
    public:
        void setup(); //初期設定
        int get_azimuth(); //方位角取得（ドリフト対策無）
        void get_cord(); //自己位置取得
        void get_speed(float dt, float accel,short i); //半自動速度計算関数
        void dir_reset(); //方向初期化
        void cord_reset(); //位置情報初期化
        void cord_custom(int x, int y); //自由位置に座標を設定可能
        void restart(); //センサーを初期化
        int get_x(); //座標のxを取得
        int get_y(); //座標のyを取得

    private:
        //調整用
        const float accel_noise = 0.12f; //動いていないと判断した際に使用するフィルタ（加速度センサーのノイズ除去）
        const float movement_border = 0.15f; //動作判定ボーダー　accel_noiseよりは高い値を使用してください
        const float adaptive_noise = 0.02f; //動いていると判断した際に使用するフィルタ
        const float accel_tweaker = 1.0f; //この値が大きければ大きいほど、小さい値が増幅される
        const float filterCoefficient = 0.95; //速度の平滑度合い　1に近いほど強い
        const float reset_border = 3; //0が何回続いたらリセットするか
        const float accel_offset_x = 1.1;
        const float accel_offset_y = 1.0; 

        int azimuth;
        // int world_x;
        // int world_y;
        int yawtweak;
        int zero_count[2];
        int ten_count; //値を10にするかを決めるやつ
        int j;
        int PoMi[2] = {1, 1}; //1は1~、0は~-1、10は0
        int first_PoMi[2] = {10, 10};
        int world_x;
        int world_y;
        bool move[2] = {false, false};
        bool collision;
        bool moving;
        float a;
        float b;
        float a_dt;
        float b_dt;
        float lowpassValue[2];
        float highpassValue[2];
        float old_accel_data[2];
        float oold_accel_data[2];
        float difcord_x;
        float difcord_y;
        float yaw_rad;
        float speed[2];
        float old_cordtime;
        float accel_bias[3] = {0.0, 0.0, 0.0};

        //自己位置推定用
        float states[6] = {0.0, 0.0};
};
