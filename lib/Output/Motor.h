#pragma once

#include <Arduino.h>


class MyMOTOR {
    public:
        void setup(); //初期設定
        void run(int movement_azimuth, int power_, int dir_azimuth); //進む方向、力（最大Powerは255）、向く方向（姿勢制御用）を引数にいれる
        int difix(int setpoint); //PID姿勢制御用
        void free(); //自由回転
        void brake(); //ブレーキ
        void limiter(int stat);

    private:
        //調整用
        float kp = 2.0; //比例 を大きくすると応答が速くなるが、振動しやすくなる
        float ki = 0.5; //積分 を大きくすると誤差が蓄積されにくくなるが、過剰補正のリスク
        float kd = 1.0; //微分 を大きくすると急激な変化を抑えられるが、ノイズの影響を受けやすい
        float pwmscale = 0.71; //補正速度
        float power_limiter = 140 / 255;

        bool power_limit = 1;
        bool PoMi; //正・負判断用
        bool difix_PoMi; //姿勢制御値の正・負判断用
        short motorPWM; //機体動作用入力値
        short difixPWM; //姿勢制御用入力値
        short power; //進行方向を参考した場合のモーター出力
        short azimuth_motor; //モーターから見た進行方向の方位角
        double integral;
        double prev_error;
        double derivative;
        const byte motor_PIN1[4] = {8, 7, 5, 3};
        const byte motor_PIN2[4] = {9, 6, 4, 2};
        const short motor_degrees[4] = {45, 135, 225, 315};
};
