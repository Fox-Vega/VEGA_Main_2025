#pragma once

#include <Arduino.h>

class MyVECTOR {
    public:
        int get_azimuth(int x, int y); //方位角を計算する関数
        float get_magnitude(int x, int y); //距離を計算する関数
        void get_cord(int azimuth, int magnitude);//座標を計算する関数
        int get_x(); //取得したx座標を返す関数
        int get_y(); //取得したy座標を返す関数
        int get_vectordegree(int ax, int ay, int bx, int by);

    private:
        int x, y;
        float theta;
        float azimuth;
};