#pragma once

#include <Arduino.h>

class GAM { //いろいろエラーが出てたから弄った結果、GAMクラス誕生☆　Gyro,Accelometer,Magneticをテキトーにもじりました
    public:
        void setup(); //初期設定
        int get_azimuth(); //方位角取得
        void get_cord(); //自己位置取得
        void get_speed(float dt, float accel,short i); //半自動計算関数
        void dir_reset(); //方向初期化
        void accel_reset();
        void cord_custom(int x, int y); //自由座標に設定可能
        int get_x(); //座標のxを取得
        int get_y(); //座標のyを取得

    private:
        //使用機体 0はオフェンス　1はディフェンス
        const short robotNUM = 0;
        //調整用
        const float filterCoefficient = 0.2; //1に近いほど値の平滑度合いが強い　低速時の感度も変化 大事
        const float adaptive_noise = 0.02f; //動作判断時に使用するフィルタ
        const float accel_noise = 0.06f; //静止判断時に使用するフィルタ
        const float accel_sparknoise = 10.0f; //スパークノイズ（個人的な呼び名）の判定基準（衝突にも流用）
        const float reset_border = 2; //加速度変化無しが何回続いたら速度をリセットするか

        const float accel_offsetp[2][2] = {{1.1, 1.0}, {1.0, 1.0}}; //+出力の倍率 x,y
        const float accel_offsetm[2][2] = {{1.0, 1.2}, {1.0, 1.0}}; //-出力の倍率 x,y

        int j;
        int stable;
        int world_x;
        int world_y;
        int azimuth;
        int yawtweak;
        int ten_count;
        int PoMi[2] = {1, 1}; //値の正負を保存する変数　1は1~、0は~-1、10は0
        int first_PoMi[2] = {10, 10}; //最初に検出された値の正負を保存する変数
        bool zero_pro = true;
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
        float states[2] = {0.0, 0.0};
        float accel_bias[2] = {0.0, 0.0};
};