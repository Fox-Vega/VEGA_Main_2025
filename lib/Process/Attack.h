#pragma once

#include <Arduino.h>

class Attack {
    public:
        void attack_();
        void b_p1();//ボールの回り込みパターン１
        void b_p2();//ボールの回り込みパターン２
        void b_p3();//ボールの回り込みパターン３
    private:
        //p1　はゴールへの突進
        //p2　はボールへ向かう
        //p3　は接線でボールの周りをまわる
        // 調整
        float appraoch_value = 1.5; //ボールの回り込みパターン１の際の角度増加値
        const int brr = 200; //回り込み半径
        const int b_r1_deg = 30; //パターン１を行う範囲(片側)
        const short b_r1speed = 100; //ボールの回り込みパターン１の速度
        const short b_r2speed = 100; //ボールの回り込みパターン２の速度
        const short b_r3speed = 100; //ボールの回り込みパターン３の速度
        const short goalcords[2] = {0, 3000};
        const short ballcatchvalue = 2000;
        const short goalspeed = 100;

        short ball_dir;
        short face;
        int movedir;
};
