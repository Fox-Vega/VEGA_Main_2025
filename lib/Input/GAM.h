#pragma once

#include <Arduino.h>

class GAM { //いろいろエラーが出てたから弄った結果、GAMクラス誕生☆　Gyro,Accelometer,Magneticをテキトーにもじりました
    public:
        /// @brief GAMセンサー（ジャイロ・加速度・磁気）の初期設定を行う
        void setup();
        /// @brief 方位角データを読み取る
        void read_azimuth();
        /// @brief 現在の方位角を取得する
        /// @return 方位角（0-359度）
        int get_azimuth();
        /// @brief 方向データを初期化する
        void dir_reset();

        /// @brief 自己位置座標を取得する
        void get_cord();
        /// @brief 速度を計算する（半自動計算関数）
        /// @param dt 時間差分
        /// @param accel 加速度
        /// @param i インデックス
        void get_speed(float dt, float accel,short i);
        /// @brief 加速度データをリセットする
        void accel_reset();
        /// @brief 自己位置座標をカスタム設定する
        /// @param x X座標
        /// @param y Y座標
        void cord_custom(int x, int y);
        /// @brief 自己位置のX座標を取得する
        /// @return X座標
        int get_x();
        /// @brief 自己位置のY座標を取得する
        /// @return Y座標
        int get_y();

    private:
        //使用機体 0はオフェンス　1はディフェンス
        const short robotNUM = 0;
        //調整用
        const float filterCoefficient = 0.2; //1に近いほど値の平滑度合いが強い　低速時の感度も変化 大事
        const float adaptive_noise = 0.02f; //動作判断時に使用するフィルタ
        const float accel_noise = 0.06f; //静止判断時に使用するフィルタ
        const float accel_sparknoise = 10.0f; //スパークノイズ（個人的な呼び名）の判定基準（衝突にも流用）
        const float reset_border = 2; //加速度変化無しが何回続いたら速度をリセットするか

        const float accel_offsetp[2][2] = {{1.0, 1.0}, {1.0, 1.0}}; //+出力の倍率 x,y 1つ目がアタッカー用
        const float accel_offsetm[2][2] = {{1.0, 1.0}, {1.0, 1.0}}; //-出力の倍率 x,y
        const int cord_offset = 10; //座標倍率

        int stable;
        int world_x;
        int world_y;
        int azimuth;
        int yawtweak;
        int ten_count;
        int sampleNUM;
        int PoMi[2] = {1, 1}; //値の正負を保存する変数　1は1~、0は~-1、10は0
        int first_PoMi[2] = {10, 10}; //最初に検出された値の正負を保存する変数
        bool j;
        bool move[2] = {false, false};
        float a_dt;
        float b_dt;
        float speed[2];
        float sample[2];
        float difcord_x;
        float difcord_y;
        float old_speed[2];
        float lowpassValue[2];
        float highpassValue[2];
        float old_accel_data[2];
        float states[2] = {0.0, 0.0};
        float accel_bias[2] = {0.0, 0.0};
        unsigned long old_cordtime;
};
