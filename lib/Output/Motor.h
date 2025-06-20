#pragma once

#include <Arduino.h>


class MyMOTOR {
    public:
        void setup(); //初期設定
        void run(int movement_azimuth, int power_, int dir_azimuth); //進む方向、力（最大Powerは255）、向く方向（姿勢制御用）を引数にいれる
        int difix(int target_azimuth); //PID姿勢制御用
        void free(); //自由回転
        void brake(); //ブレーキ
        int get_azimuth();
        int get_magnitude();
        void stabilization(bool stat);
        void move(bool stat);

    private:
        //調整用
        const float kp = 0.8; //比例 を大きくすると応答が速くなるが、振動しやすくなる
        const float kd = 15.0; //微分 を大きくすると急激な変化を抑えられるが、ノイズの影響を受けやすい
        const int pwmlimit = 130;
        const float pwmscale = 1.0;
        // const int motor_border = 90; //モーターが回っていると認識するボーダー
        const int motor_border = 100; //モーターが回っていると認識するボーダー
        const float pwm_tweaker[2] = {1.0, 1.0}; //1つ目が＋（右回り）　2つ目が-（左回り）　姿勢制御

        int prev_azimuth;
        int motor_azimuth;
        int motor_magnitude;
        bool PoMi; //正・負判断用
        bool motor_move = 1;
        bool motor_stat;
        bool motor_stabilization;
        bool old_motor_stat;
        short power; //進行方向を参考した場合のモーター出力
        short motorPWM; //機体動作用入力値
        short difixPWM; //姿勢制御用入力値
        short azimuth_motor; //モーターから見た進行方向の方位角
        double integral;
        double prev_error;
        double derivative;
        const byte motor_PIN1[4] = {9, 7, 5, 3};
        const byte motor_PIN2[4] = {8, 6, 4, 2};
        const short motor_degrees[4] = {45, 135, 225, 315};
        unsigned long  lastupdate;
};
