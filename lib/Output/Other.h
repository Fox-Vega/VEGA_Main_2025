#pragma once

#include <Arduino.h>


// 色の参考
// int red[3] = {255, 0, 0};
// int Yellow[3] = {255, 255, 0};
// int green[3] = {0, 255, 0};
// int blue[3] = {0, 0, 255};
// int Purple[3] = {255, 0, 255};
// int White[3] = {255, 255, 255};

class MyPIXEL {
    public:
        void setup(); //初期設定
        void brightness(int brightness); //ネオピクの明るさ調整　999にすると既定値になる

        //ピクセル番号は時計回りで 0~15
        void uni(int PIXELNUM, int red, int green, int blue); //単ピクセル点灯
        void multi(int PIXELNUMstart, int PIXELNUMend, int red, int green, int blue); //連続ピクセル点灯
        void closest(int azimuth, int red, int green, int blue, int num);
        void rainbow();
        void show();
        void shows();
        void clear(); //全消灯

    private:
        //調整用
        const byte usePIXEL = 1; //ネオピクセルの使用有無
        const float PIXELbrightness = 150; //最大で255
        
        int step_num = 0;
        int PIXELNUMstart;
};

class MyBUZZER {
    public:
        void setup();
        void start(int BUZZERnote, int BUZZERduration); //音程、持続時間を指定して音を鳴らす　持続時間を999にすると無制限
        void preset(int BUZZERpresetNUM); //既定の音を鳴らす

    private:
        const bool usebuzzer = 1;
        const byte BUZZER_PIN = 47;
};
