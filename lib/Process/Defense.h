#pragma once


#include <time.h>
#include <Arduino.h>
#include <Input.h>
#include <Output.h>
#include <Process.h>
#include <AIP.h>
#include <math.h>
#include <string.h>

#define linedist 10
#define linecentor 0.1
class DEFENCE {
public:
    void setup();
    void defense_();
private:
struct Dline
    {
        int deg=0;
        int dist=0;
        bool detect=false;
    };
    struct Dball
    {
        int deg=0;
        int dist=0;
        int flag=0; // 0:左, 1:真ん中, 2:右, 3:後ろ
        bool detect=false;
    };
    int go_ang=0;
};