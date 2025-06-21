#pragma once
#include <Arduino.h>

struct data
{
    int azimuth;
    int dist;
    float x;
    float y;
    bool detect;
};


class Defense {
    public:
        void setup(void);
        void defense_(void);
        void p1(void);
        void keeper_dash(void);
        void GoBackLine(void);
    private:
    const int ball_max = 900;
    const int motor_max = 255;
    int go_ang;
    int line_history[3][300];
    int ball_history[3][300];
    int line_history_index=0;
    int ball_history_index=0;
    void MyUI(int mode);
    void MyUI_clear(void);
    void get_vector(void);
};