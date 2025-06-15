#ifndef OKOMEONIGIRI_H
#define OKOMEONIGIRI_H

//コピペ用
//==================================================
//==============================

//  OKOMEONIGIRI.h
//  Created by okomeonigiri
#include <stdio.h>
#include <limits.h>

#ifdef RCJJ_VEGA
#include "AIP/AIP.h"
#include "AIP/General.h"
#include "AIP/MyVector.h"
#include "AIP/Timer.h"

#include "Input/Ball.h"
#include "Input/GAM.h"
#include "Input/Input.h"
#include "Input/Line.h"
#include "Input/MySwitch.h"

#include "Output/Motor.h"
#include "Output/Other.h"
#include "Output/Output.h"

#include "Process/Attack.h"
#include "Process/Defense.h"
#include "Process/Process.h"
#include "Process/Test.h"
#endif



//printf format for arudino Serial print
//==================================================

// Use snprintf to format strings and print them to Serial
// This allows for formatted output similar to printf in standard C

//buf size:256  normal
#define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
//buf size:128  short
#define printf_s_short(fmt, ...) ({ char buf[128]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
//buf size:512  long
#define printf_s_long(fmt, ...) ({ char buf[512]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
//buf size:custom  // Use a custom buffer size defined by the user
#define printf_s_custom(bufsize,fmt, ...) ({ char buf[bufsize]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
//buf size:SIZE_MAX  // Use the maximum size of a buffer, which is defined by SIZE_MAX
#define printf_s_max(fmt, ...) ({ char buf[SIZE_MAX]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })

//==================================================

//A for loop that allows you to easily use a for loop with just a single number.
#define floop(n) for(size_t i = 0; i < n; ++i)
//A floop that allows you to use any variable name you like as the loop counter.
#define floop_id(var, n) for(int var = 0; var < (n); ++var)

#define get_min(a, b) ((a) < (b) ? (a) : (b))
#define get_max(a, b) ((a) > (b) ? (a) : (b))

int calculate_deg(char mode, int num1, int num2){
    switch(mode){
        case 'a': return (num1+num2>=360)?(num1+num2)%360:(num1+num2); // 加算
        case 'r': return (num1+180)%360; // 180度回転
        case 's': return (num1-num2<0)?(num1-num2+360)%360:(num1-num2); // 減算
        case 'A': return ((num1+num2)/2>=360)?((num1+num2)/2)%360:((num1+num2)/2); // 平均
        case 'm': return (num1<num2)?num1:num2; // 最小値
        case 'M': return (num1>num2)?num1:num2; // 最大値
        case 'd': return (num1-num2>=0)?(num1-num2):(num2-num1); // 差の絶対値
        case 'n': return (num1%360+360)%360; // 正規化
        case 'z': return (450-num1)%360; // azimuth→theta変換（num1のみ使用）
        case 't': return (450-num1)%360; // theta→azimuth変換（num1のみ使用）
        case 'q': return (num1+90)%360; // 90度回転（num1のみ使用）
        case 'Q': return (num1+270)%360; // -90度回転（num1のみ使用）
        default: return 999;
    }
}
/*
mode
'a' : 加算[num1+num2] ex) 240+230=10
'r' : 180度回転[num1+180] ex) 45+180=225
's' : 減算[num1-num2] ex) 30-50=340
'A' : 平均[(num1+num2)/2] ex) 60+120=90
'm' : 最小値[num1, num2の小さい方] ex) 20,80=20
'M' : 最大値[num1, num2の大きい方] ex) 20,80=80
'd' : 差の絶対値[|num1-num2|] ex) 10-350=340
'n' : 正規化[0～359度に変換] ex) -20=340
'z' : azimuth→theta変換[num1のみ使用] ex) 90→0
't' : theta→azimuth変換[num1のみ使用] ex) 0→90
'q' : 90度回転[num1のみ使用] ex) 45+90=135
'Q' : -90度回転[num1のみ使用] ex) 45+270=315
default: 999
*/
int cal_deg(char mode, int num1, int num2){return calculate_deg(mode, num1, num2);}
#endif //OKOMEONIGIRI_H