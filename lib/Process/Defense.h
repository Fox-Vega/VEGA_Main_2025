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

        // line
        int line_azimuth;
        int line_dist;
        float line_x;
        float line_y;
        bool line_detect;

        const int ball_near = 130;
        const int ball_max = 900;
        const int motor_max = 255;
        int go_ang;
        int lastdetect = 0;
        int line_history[3][50];
        int ball_history[3][50];
        int line_history_index=0;
        int ball_history_index=0;
        void MyUI(int mode);
        void MyUI_clear(void);
        void get_vector(void);
};