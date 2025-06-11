#pragma once
#include <Output.h>
#include <Arduino.h>
#include <AIP.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>






class LINE {
public:
    // 呼び出し関数
    //==============================
    void setup();//初期設定（必ずやってね☆）
    int get_azimuth();//センサの角度を取得　
    int get_reverse_azimuth();
    int get_magnitude();//センサの距離を取得
    bool read();//センサの状態を読み取る（true:ラインあり, false:ラインなし
    void serial_print(void);
    int calculate_deg(char mode, int num1, int num2);//角度計算
        /*
        使い方
        a:加算[num1+num2] ex) 240+230=10
        s:減算[num1-num2] ex) 30-45=345
        r:反転[reverse num1] ex)30→150  [-180]
        A:平均[num1+num2/2] ex) 240+30=135
        ※文字は''で囲ってね
        */
    int cal_deg(char mode, int num1, int num2){return calculate_deg(mode, num1, num2);}//角度計算（関数の中で関数呼び出してるから速度が必要なときにはわざわざ上を入力した方が良いかも）
        /*
        関数の中で関数呼び出してるから速度が必要なときにはわざわざ上を入力した方が良いかも
        */
    //==============================

private:
        const int selectA = 22;
        const int selectB = 24;
        const int selectC = 26;

        #define readPin1 A13
        #define readPin2 A11
        #define readPin3 A9

        //ラインの数(変えるか…？)
        #define NUMLines 24
        const int sensordist = 1000; // センサの距離（cm）
        #define lineDetect 500
        #define printf_s SERIAL_PRINTF
        #define SERIAL_PRINTF(fmt, ...) ({ char buf[512]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })


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
    // 内部変数;
    bool line_status[NUMLines] = {0};
    int line_value[NUMLines] = {0};
    int line_memory[NUMLines] = {0}; // ライン検出の履歴
    int line_detect[4] = {999}; // ライン検出角度リスト
    int count = 0;
    int progress = 0;
    int returnX = 0; // 角度計算用の戻り値
    int returnY = 0; // 角度計算用の戻り値
    int linesituation = 0;/*
        linesituationの値
        0:ラインなし
        1:直線
        2:曲線
        3:角
        */
    // 内部処理関数
    int get_linedeg();
    int get_line_dist(int deg1, int deg2);
    void print_pizel();
    void get_claster();
};
