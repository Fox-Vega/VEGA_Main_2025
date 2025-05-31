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

#define defence defense//スペルミス多いのでね、
#define Defence Defense//一応、
#define DEFENCE DEFENSE//作っておこうかと。

#define linedist 10
#define linecentor 0.1


    struct data
    {
        int theata;
        int dist;
        float x;
        float y;
        bool detect;
    };


class Defense {
    public:
        void setup(void);
        void defense_(void);
        void get_vector_Ball(void);
        void get_vector_Line(void);
    private:
};