#pragma once

#include <Arduino.h>


class MyMOTOR {
    public:
        void setup(); //初期設定
        void run(int movement_azimuth, int power_, int dir_azimuth); //進む方向、力（最大Powerは255）、向く方向（姿勢制御用）を引数にいれる
        int difix(int target_azimuth); //PID姿勢制御用
        void free(); //自由回転
        void brake(); //ブレーキ
        void limiter(bool stat); //0にすると制限なし移動速度が出る　モーター摩耗が激しくなるため、OOBを防ぐときにのみ解除するように。

    private:
        //調整用
        const float kp = 0.06; //比例 を大きくすると応答が速くなるが、振動しやすくなる
        const float ki = 0.1; //積分 を大きくすると誤差が蓄積されにくくなるが、過剰補正のリスク
        const float kd = 0.2; //微分 を大きくすると急激な変化を抑えられるが、ノイズの影響を受けやすい
        const float pwmscale = 10.0; //姿勢補正速度
        const int pwmlimit = 140;
        const int difixlimit = 60;
        const float integrallimit = 20;

        bool PoMi; //正・負判断用
        short power; //進行方向を参考した場合のモーター出力
        short motorPWM; //機体動作用入力値
        short difixPWM; //姿勢制御用入力値
        short azimuth_motor; //モーターから見た進行方向の方位角
        float lastupdate;
        double integral;
        double prev_error;
        double derivative;
        const byte motor_PIN1[4] = {9, 7, 5, 3};
        const byte motor_PIN2[4] = {8, 6, 4, 2};
        const short motor_degrees[4] = {45, 135, 225, 315};
};
