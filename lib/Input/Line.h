#pragma once
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <AIP.h>
#include <Output.h>

class LINE {
    public:
        //呼び出し可能
        void setup(); // ピン設定を行うセットアップ関数（制御の初めにやっといてー）
        int get_azimuth(); // ラインセンサの値を取得する関数＞＞＞呼び出し＜＜＜
        int read(); // ラインセンサの値を取得する関数>>>呼び出し（boolを返す）<<<　　ex)if(line.read()) 
        int get_linedeg(); // ラインセンサの角度を取得する関数
        void add_linedeg(int num,int num2); // ラインセンサの角度を追加する関数
        int get_dist(int linedeg ,int linedeg2);
        void print_pizel();
        int get_deg(char mode, int num1, int num2);
        /*
        A:加算（slow）[num1+num2] ex) 240+230=10
        a:加算（fast）[num1+num2] ex) 240+230=10
        S:減算（slow）[num1-num2] ex) 30-45=345
        s:減算（fast）[num1-num2] ex) 30-45=345
        r:反転（fast only）[reverse num1] ex)30→150  [-180]
        */
        int calculate_deg(char mode, int num1, int num2);//角度計算

    private:
        const int sensordist = 10; 
        #define true 1
        #define false 0
        #define NUMLines 24

        // マルチプレクサのアナログ入力ピン
        #define readPin1 A13
        #define readPin2 A11
        #define readPin3 A9


        // マルチプレクサのセレクトピン
        const int selectA = 22;
        const int selectB = 24;
        const int selectC = 26;

        int Line_deg; // ラインセンサの角度（戻り値用に使用する可能性あり）
        int Line_dis; // ラインセンサの距離（戻り値用に使用する可能性あり）

        // ラインセンサの角度（センサ番号と角度の対応）
        const int Line_deg_list_24[24] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};

        // ラインセンサの値を格納する2次元配列（センサ番号と値を保持）
        int Line_memory[24][2] = {
            {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 10}, {5, 20}, {6, 30}, {7, 50},
            {8, 30}, {9, 20}, {10, 10}, {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0},
            {16, 0}, {17, 0}, {18, 0}, {19, 0}, {20, 0}, {21, 0}, {22, 0}, {23, 0}};

        // マルチプレクサの設定値（二進数のリスト）
        int BinaryNum[24][3] = {
            {0, 0, 0},
            {0, 0, 1},
            {0, 1, 0},
            {0, 1, 1},
            {1, 0, 0},
            {1, 0, 1},
            {1, 1, 0},
            {1, 1, 1},
            {0, 0, 0},
            {0, 0, 1},
            {0, 1, 0},
            {0, 1, 1},
            {1, 0, 0},
            {1, 0, 1},
            {1, 1, 0},
            {1, 1, 1},
            {0, 0, 0},
            {0, 0, 1},
            {0, 1, 0},
            {0, 1, 1},
            {1, 0, 0},
            {1, 0, 1},
            {1, 1, 0},
            {1, 1, 1},
        };
        bool line_status[NUMLines]; // ラインセンサの状態を保持する配列
        int line_detect[4];
        int count = 0; // センサの状態を保存する時の場所の参照
        int progress = 0;// 進捗を保存する変数


        short startPIXELs[5] = {1, 3, 15, 7, 11};
};



