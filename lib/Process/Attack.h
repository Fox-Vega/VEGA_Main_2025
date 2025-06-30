#pragma once

#include <Arduino.h>

class Attack {
    public:
        void attack_();

        void b_p1();
        void b_p2();
        void b_p3();
        void b_p4();
    private:
        //p1　はボールを捕獲ゾーンに入れる
        //p2　は接線を計算してボールへ向かう
        //p3　は接線でボールの周りをまわる
        //p4　はゴールムーブ

        // 調整
        const float appraoch_value = 1.4; //ボールの回り込みパターン１の際の角度増加値
        const float crecent_p2 = 1.6; //p2回り込み半径増幅値
        const int brr = 400; //p3回り込み半径
        const int b_r1_deg = 25; //パターン１を行う範囲(片側)
        const short b_r1speed = 100; //ボールの回り込みパターン１の速度
        const short b_r2speed = 90; //ボールの回り込みパターン２の速度
        const short b_r3speed = 90; //ボールの回り込みパターン３の速度
        const short b_r4speed = 0;
        const short avoid_speed = 115; //ライン回避


        bool catch_stat = 0;
        short ball_dir;
        short face;
        int movedir;
};
