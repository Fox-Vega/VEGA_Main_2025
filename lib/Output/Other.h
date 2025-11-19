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

class MyPIXEL {
    public:
        /// @brief NeoPixelの初期設定を行う
        void setup();
        /// @brief NeoPixelを点灯する
        void on();
        /// @brief NeoPixelの明るさを調整する
        /// @param brightness 明るさ（999で既定値）
        void brightness(int brightness);

        /// @brief 単一ピクセルを点灯する（ピクセル番号は時計回りで0~15）
        /// @param PIXELNUM ピクセル番号
        /// @param red 赤成分（0-255）
        /// @param green 緑成分（0-255）
        /// @param blue 青成分（0-255）
        void uni(int PIXELNUM, int red, int green, int blue);
        /// @brief 連続ピクセルを点灯する
        /// @param PIXELNUMstart 開始ピクセル番号
        /// @param PIXELNUMend 終了ピクセル番号
        /// @param red 赤成分（0-255）
        /// @param green 緑成分（0-255）
        /// @param blue 青成分（0-255）
        void multi(int PIXELNUMstart, int PIXELNUMend, int red, int green, int blue);
        /// @brief 指定方位角に最も近いピクセルを点灯する
        /// @param azimuth 方位角
        /// @param red 赤成分（0-255）
        /// @param green 緑成分（0-255）
        /// @param blue 青成分（0-255）
        /// @param num 点灯数
        void closest(int azimuth, int red, int green, int blue, int num);
        /// @brief 指定角度範囲のピクセルを点灯する
        /// @param start_angle 開始角度（0-359度）
        /// @param end_angle 終了角度（0-359度）
        /// @param red 赤成分（0-255）
        /// @param green 緑成分（0-255）
        /// @param blue 青成分（0-255）
        void angle_range(int start_angle, int end_angle, int red, int green, int blue);
        /// @brief レインボーパターンを表示する
        void rainbow();
        /// @brief ピクセル表示を更新する
        void show();
        /// @brief ピクセル表示を更新する（別名）
        void shows();
        /// @brief 全ピクセルを消灯する
        void clear();
        /// @brief 全ピクセルを消灯する（別名）
        void clears();
        /// @brief ピクセル使用の有効/無効を設定する
        /// @param stat true:有効 false:無効
        void use_pixel(bool stat);
        /// @brief ピクセル使用状態を取得する
        /// @return true:有効 false:無効
        bool stat();

        /// @brief HEX6カラーコードをRGBに変換する
        /// @param hex HEXカラーコード
        /// @param red 赤成分（出力）
        /// @param green 緑成分（出力）
        /// @param blue 青成分（出力）
        void HEX6toRGB(String hex, int red, int green, int blue);
        /// @brief HEX8カラーコードをRGBAに変換する
        /// @param hex HEXカラーコード
        /// @param red 赤成分（出力）
        /// @param green 緑成分（出力）
        /// @param blue 青成分（出力）
        /// @param alpha アルファ成分（出力）
        void HEX8toRGBA(String hex, int red, int green, int blue, int alpha);
        /// @brief RGBA成分を取得する
        /// @param get 取得する成分（'r','g','b','a'）
        /// @return 成分値
        double getRGBA(char get);

    private:
        //調整用
        const float PIXELbrightness = 50; //最大で255

        int step_num = 0;
        int PIXELNUMstart;
        bool Test = 0;
        bool usePIXEL;
        const byte PIXELPIN = 45; // Pin where the NeoPixel ring is connected
        const byte NUMPIXEL = 16; // NUMber of pixels in the strip
        Adafruit_NeoPixel PIXEL = Adafruit_NeoPixel(NUMPIXEL, PIXELPIN, NEO_GRB + NEO_KHZ800);

        int R;
        int G;
        int B;
        int A;
};

class MyBUZZER {
    public:
        /// @brief ブザーの初期設定を行う
        void setup();
        /// @brief ブザーを鳴らす
        /// @param BUZZERnote 音程（Hz）
        /// @param BUZZERduration 持続時間（ms、999で無制限）
        void start(int BUZZERnote, int BUZZERduration);
        /// @brief ブザーを停止する
        void stop();
        /// @brief プリセット音を鳴らす
        /// @param BUZZERpresetNUM プリセット番号
        void preset(int BUZZERpresetNUM);

    private:
        const bool useBUZZER = 1;
        const byte BUZZER_PIN = 47;
};
