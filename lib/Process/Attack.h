#pragma once

#include <Arduino.h>

class Attack {
    public:
        void attack_();

        //p1　はゴールへの突進
        //p2　はボールへ向かう
        //p3　は接線でボールの周りをまわる
        //p4　はゴール突進
        void b_p1();
        void b_p2();
        void b_p3();
        void b_p4();
    private:
        // 調整
        float appraoch_value = 1.6; //ボールの回り込みパターン１の際の角度増加値
        const int brr = 370; //回り込み半径
        const int b_r1_deg = 30; //パターン１を行う範囲(片側)
        const short b_r1speed = 120; //ボールの回り込みパターン１の速度
        const short b_r2speed = 120; //ボールの回り込みパターン２の速度
        const short b_r3speed = 100; //ボールの回り込みパターン３の速度
        const short b_r4speed = 120;
        const short ball_catch[2] = {200, 260}; //１番目が最低値200　２番目が最高値260

        const int goalcord = 300;

        bool catch_stat = 0;
        short ball_dir;
        short face;
        int movedir;
};
