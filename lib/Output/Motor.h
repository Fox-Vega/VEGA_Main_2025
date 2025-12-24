#pragma once

#include <Arduino.h>


class MyMOTOR {
    public:
        void setup(); //初期設定
        void run(int movement_azimuth, int power_, int dir_azimuth); //移動する（方向、パワー（０～２５５）、姿勢制御の方向）
        int difix(int target_azimuth); //姿勢制御出力を取得
        void free(); //空転
        void brake(); //ブレーキ
        int get_azimuth(); //現在の移動方向を取得
        int get_magnitude(); //現在のパワーを取得
        void stabilization(bool stat); //姿勢制御を行う　有効・無効
        void move(bool stat); //モーターへ信号を送る　有効・無効

    private:
        //調整用
        const float kp = 2.6; //比例 を大きくすると応答が速くなるが、振動しやすくなる
        const float kd = 0.176; //微分 速度の上昇を抑える
        const int pwmlimit = 230;
        const int stabimit = 100;
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
