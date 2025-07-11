#pragma once
#include <Arduino.h>

class Defense {
    public:
        void setup(void);
        void defense_(void);
        void p1(void);//normal line
        void p2(void);//on corner
        void p3(void);//on next to goal
        void line_(void);//trace test line
        void ball_(void);
        void trace(void);
        void keeper_dash(void);
        void GoBackLine(void);
    private:
        //調整


        // ball
        const double bmgn =1;
        const double bmmgn=0.1;
        int ball_azimuth;
        int ball_dist;
        float ball_x;
        float ball_y;
        bool ball_detect;
        const int category_width = 30; // ボールの位置分類の幅

        // line
        int line_azimuth;
        int line_dist;
        float line_x;
        float line_y;
        bool line_detect;

        // others
        int r_azimuth;

        const int ball_near = 130;
        const int ball_max = 900;
        const int motor_max = 255;
        int setup_f=0; // 初期化フラグ
        int go_ang;
        int line_go_ang;
        int line_fb;
        int line_power;
        int move_x;
        int move_y;
        int move_power;
        int ball_power = 50;
        int ball_go_ang = 90;
        int category = 0; // ボールの位置分類
        int lastdetect = 180;
        int line_history[3][50];
        int ball_history[3][50];
        int line_history_index=0;
        int ball_history_index=0;
        int gotVector=0;
        void MyUI(int mode);
        void MyUI_clear(void);
        void get_vector(void);
};