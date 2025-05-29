#pragma once

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
//#include <Arduino.h>
#include <Input.h>
#include <Output.h>
#include <Process.h>
#include <AIP.h>

#define linedist 10
#define linecentor 0.1
class LINE {
public:
    void setup();
    void defence_();
    struct Dline
    {
        int linne_deg=0;
        int line_dist=0;
        bool line_detect=false;
    };
    struct Dball
    {
        int ball_deg=0;
        int ball_dist=0;
        int ball_flag=0; // 0:左, 1:真ん中, 2:右, 3:後ろ
        bool ball_detect=false;
    };
private:
    int go_ang=0;
};