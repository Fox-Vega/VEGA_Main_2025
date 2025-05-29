#pragma once
#include <Output.h>
#include <Arduino.h>
#include <AIP.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUMLines 24
#define sensordist 10
#define LINEDETECT 5

class LINE {
public:
    // 呼び出し関数
    void setup();
    int get_azimuth();
    int get_dist();
    int calculate_deg(char mode, int num1, int num2);//角度計算
        /*
        使い方
        A:加算（slow）[num1+num2] ex) 240+230=10
        a:加算（fast）[num1+num2] ex) 240+230=10
        S:減算（slow）[num1-num2] ex) 30-45=345
        s:減算（fast）[num1-num2] ex) 30-45=345
        r:反転（fast only）[reverse num1] ex)30→150  [-180]
        */
    int cal_deg(char mode, int num1, int num2);//上の略、結果や引数は同じ
        /*
        関数の中で関数呼び出してるから速度が必要なときにはわざわざ上を入力した方が良いかも
        */
private:
    // --- ピン定義 ---
    struct Pins {
        // セレクトピン
        const int selectA = 22;
        const int selectB = 24;
        const int selectC = 26;
        // リードピン
        const int readPin1 = A13;
        const int readPin2 = A11;
        const int readPin3 = A9;
    } pins;
    // バイナリ配列（縦3列）
    const int BinaryNum[24][3] = {
        {0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1},
        {0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1},
        {0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}
    };
    // センサ角度リスト
    const int Line_deg_list_24[NUMLines] = {
        0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165,
        180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345
    };
    // 内部変数
    int Line_memory[NUMLines][2] = {0};
    bool line_status[NUMLines] = {0};
    int line_detect[NUMLines] = {0};
    int count = 0;
    int progress = 0;
    // 内部処理関数
    int read();
    int get_linedeg();
    int get_line_dist(int deg1, int deg2);
    void print_pizel();
};