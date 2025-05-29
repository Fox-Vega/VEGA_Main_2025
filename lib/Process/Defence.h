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
 void defence();
private:
int go_ang=0;

};