#pragma once

#include <Arduino.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

class Attack {
    public:
        void attack_();
        void b_p1();//ボールの回り込みパターン１
        void b_p2();//ボールの回り込みパターン２
        void b_p3();//ボールの回り込みパターン３
    private:
        // 調整
        float appraoch_value = 1.5; //ボールの回り込みパターン１の際の角度増加値
        const int brr = 100; //回り込み半径
        const int b_r1_deg = 10; //ボールの回り込みパターン１を行う範囲(片側)
        const short b_r1speed = 255; //ボールの回り込みパターン１の速度
        const short b_r2speed = 200; //ボールの回り込みパターン２の速度
        const short b_r3speed = 100; //ボールの回り込みパターン３の速度
        const short goalcords[2] = {0, 3000};
        const short ballcatchvalue = 1200;
        const short goalspeed = 200;

        short ball_dir;
        short face;
        float movedir;
};
