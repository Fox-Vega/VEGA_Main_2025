#pragma once
#include <Arduino.h>

class Defense {
    public:
        void setup(void);
        void defense_(void);
        void line_calibration(void);
    private:
        // ball
        const int ball_max_X = 300;
        const int catch_ball_X = 20;
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
        short line_type;

        // others
        const int line_rate =1;
        const int line_p2_trace_trigger = 7;
        int r_azimuth;

        int setup_f=0;
        int go_ang;
        int line_go_ang;
        int line_fb;
        int line_power;
        int move_x;
        int move_y;
        int move_power;
        int ball_power = 50;
        int ball_go_ang = 90;
        int lastdetect = 180;
        int line_history[3][50];
        int ball_history[3][50];
        int line_history_index=0;
        int ball_history_index=0;
        int gotVector=0;
        void get_vector(void);
        void p1(void);//on horizontal line
        void p2(void);//on corner
        void p3(void);//on vetical line
        void p4(void);//Goback line
        void trace(void);
        void keeper_dash(void);
        void debug1(void);
};