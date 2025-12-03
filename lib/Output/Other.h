#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


// 色の参考
// int red[3] = {255, 0, 0};
// int Yellow[3] = {255, 255, 0};
// int green[3] = {0, 255, 0};
// int blue[3] = {0, 0, 255};
// int Purple[3] = {255, 0, 255};
// int White[3] = {255, 255, 255};

// ピクセルは０～１５番まで
// RGBは各０～２５５まで

class MyPIXEL {
    public:
        void setup(); //初期設定
        void brightness(int brightness); //０～２５５明るさを設定（９９９＝規定値）

        void uni(int PIXELNUM, int red, int green, int blue); //指定のピクセルを点灯する
        void multi(int PIXELNUMstart, int PIXELNUMend, int red, int green, int blue); //指定範囲のピクセルを点灯する
        void closest(int azimuth, int red, int green, int blue, int num); //指定角度に指定個数のピクセルを光らせる
        void range(int start, int end, int red, int green, int blue); //指定した角範囲のピクセルを点灯する
        void rainbow(); //パリピ
        void show(); //ピクセルを表示
        void shows(); //ピクセルを強制表示（重たい（clearsのお供に））
        void clear(); //ピクセルを消去
        void clears(); //ピクセルを強制消去（重たい）
        void use_pixel(bool stat); //使用・不使用　の設定
        bool stat(); //現在の使用設定を確認

    private:
        //調整用
        const float PIXELbrightness = 50; //最大で255

        int step_num = 0;
        int PIXELNUMstart;
        bool Test = 0;
        bool usePIXEL;
        const byte PIXELPIN = 45; // Pin where the NeoPixel ring is connected
        const byte NUMPIXEL = 16; // NUM of pixels in the strip
        Adafruit_NeoPixel PIXEL = Adafruit_NeoPixel(NUMPIXEL, PIXELPIN, NEO_GRB + NEO_KHZ800);

        int R;
        int G;
        int B;
        int A;
};

class MyBUZZER {
    public:
        void setup(); //初期設定
        void start(int BUZZERnote, int BUZZERduration); //再生（音、持続時間（９９９＝無制限））
        void stop(); //停止
        void preset(int BUZZERpresetNUM); //プリセットを再生

    private:
        const bool useBUZZER = 1;
        const byte BUZZER_PIN = 47;
};
