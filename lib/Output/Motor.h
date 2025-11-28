#pragma once

#include <Arduino.h>


class MyMOTOR {
    public:
        /// @brief モーターの初期設定を行う
        void setup();
        /// @brief モーターを制御して移動する（姿勢制御あり）
        /// @param movement_azimuth 進行方向（0-359度）
        /// @param power_ パワー（0-255）
        /// @param dir_azimuth 向く方向（姿勢制御用）
        void run(int movement_azimuth, int power_, int dir_azimuth);
        /// @brief PID姿勢制御の補正値を計算する
        /// @param target_azimuth 目標方位角
        /// @return 補正値
        int difix(int target_azimuth);
        /// @brief モーターをフリー状態にする
        void free();
        /// @brief モーターにブレーキをかける
        void brake();
        /// @brief モーターの現在方位角を取得する
        /// @return 方位角
        int get_azimuth();
        /// @brief モーターの出力強度を取得する
        /// @return 出力強度
        int get_magnitude();
        /// @brief 姿勢制御の有効/無効を設定する
        /// @param stat true:有効 false:無効
        void stabilization(bool stat);
        /// @brief モーター動作の有効/無効を設定する
        /// @param stat true:有効 false:無効
        void move(bool stat);

    private:
        //調整用
        const float kp = 2.6; //比例 を大きくすると応答が速くなるが、振動しやすくなる
        const float kd = 0.176; //微分 を大きくするとどうなるかわからん
        const int pwmlimit = 200;
        const int stabimit = 170;
        const float pwmscale = 1.0;



        int motor_power_[4];
        int prev_azimuth;
        int motor_azimuth;
        int motor_magnitude;
        bool PoMi; //正・負判断用
        bool motor_move = 1;
        bool motor_stabilization;
        bool old_motor_stat;
        short h;
        short power; //進行方向を参考した場合のモーター出力
        short max_power;
        short motorPWM; //機体動作用入力値
        short difixPWM; //姿勢制御用入力値
        short azimuth_motor; //モーターから見た進行方向の方位角
        double pp = 0.000;
        double integral;
        double prev_error;
        double derivative;
        const byte motor_PIN1[4] = {9, 7, 5, 3};
        const byte motor_PIN2[4] = {8, 6, 4, 2};
        const short motor_degrees[4] = {45, 135, 225, 315};
        unsigned long lastupdate;
};
