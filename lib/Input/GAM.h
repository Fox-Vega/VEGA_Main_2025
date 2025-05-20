#pragma once

#include <Arduino.h>

class GAM { //いろいろエラーが出てたから弄った結果、GAMクラス誕生☆　Gyro,Accelometer,Magneticをテキトーにもじりました
    public:
        void setup(); //初期設定
        int get_azimuth(); //方位角取得
        void get_cord(); //自己位置取得
        void get_speed(float dt, float accel,short i); //半自動計算関数
        void dir_reset(); //方向初期化
        void cord_reset(); //位置情報初期化
        void cord_custom(int x, int y); //自由座標に設定可能
        void restart(); //センサー完全初期化
        int get_x(); //座標のxを取得
        int get_y(); //座標のyを取得

    private:
        //調整用
        const float movement_border = 0.15f; //動作判定ボーダー　accel_noiseより大きい値を使用するのがおすすめ
        const float adaptive_noise = 0.02f; //動作判断時に使用するフィルタ
        const float accel_noise = 0.12f; //静止判断時に使用するフィルタ
        const float accel_tweaker = 0.4f; //この値が大きければ大きいほど、小さい値が増幅される(低速時加速度増幅目的)
        const float filterCoefficient = 0.95; //1に近いほど値の平滑度合いが強い
        const float reset_border = 3; //加速度変化無しが何回続いたら速度をリセットするか
        const float accel_offset_x = 1.1; //x軸出力の倍率
        const float accel_offset_y = 1.0; //y軸出力の倍率

        int j;
        int world_x;
        int world_y;
        int azimuth;
        int yawtweak;
        int ten_count;
        int PoMi[2] = {1, 1}; //値の正負を保存する変数　1は1~、0は~-1、10は0
        int first_PoMi[2] = {10, 10}; //最初に検出された値の正負を保存する変数
        bool move[2] = {false, false};
        float a;
        float b;
        float a_dt;
        float b_dt;
        float speed[2];
        float difcord_x;
        float difcord_y;
        float old_cordtime;
        float old_speed[2];
        float lowpassValue[2];
        float highpassValue[2];
        float old_accel_data[2];
        float oold_accel_data[2];
        float states[2] = {0.0, 0.0};
        float accel_bias[2] = {0.0, 0.0};
};
