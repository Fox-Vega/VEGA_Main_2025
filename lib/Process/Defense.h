#pragma once

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <Arduino.h>
#include <Input.h>
#include <Output.h>
#include <AIP.h>
#include <math.h>
#include <string.h>
#include <Line.h>

#define linedist 10
#define linecentor 0.1

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
        void Dline_not(void);
    private:
    int go_ang;
    void MyUI(int mode);
    void MyUI_clear(void);
    void get_vector_Ball(void);
    void get_vector_Line(void);
    void get_vector(void) {
        get_vector_Ball();
        get_vector_Line();
    };
};