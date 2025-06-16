//line.h

#pragma once

#include"Output.h"
#include <Arduino.h>
#include <stdlib.h>
#include <math.h>

class LINE {
    public:
        // 呼び出し関数
        //==============================
        void setup();//初期設定（必ずやってね☆）
        int get_azimuth();//センサの角度を取得　
        int get_reverse_azimuth();
        int get_magnitude();//センサの距離を取る
        bool read();//センサの状態を読み取る（true:ラインあり, false:ラインなし
        void serial_print(void);
        //==============================
    private:
            const int selectA = 22;
            const int selectB = 24;
            const int selectC = 26;

            // #define readPin1 A13
            // #define readPin2 A11
            // #define readPin3 A9
            #define readPin1 A11
            #define readPin2 A13
            #define readPin3 A9

            //ラインの数(変えるか…？)
            #define NUMLines 24
            const int sensordist = 22;
            #define lineDetect 500
        // バイナリ配列（縦3列）
        const int BinaryNum[24][3] = {
            {0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1},
            {0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1},
            {0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}
        };
        // センサ角度リスト
        const int Line_deg_list_24[NUMLines] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
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
        int get_dist(int deg1, int deg2,int dist);
        void print_pizel();
        void get_claster();
};
