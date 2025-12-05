#pragma once

#include <Arduino.h>

class MyVECTOR {
    public:
        float get_azimuth(float x, float y); //ベクトルの方位角を取得
        float get_magnitude(float x, float y); //ベクトルのマグニチュードを取得
        void get_cord(float azimuth, float magnitude); //方位角とマグニチュードからベクトルの座標を取得
        float get_x(); //ベクトルのＸ座標を取得
        float get_y(); //ベクトルのＹ座標を取得
        float get_vectordegree(float ax, float ay, float bx, float by); //ＡベクトルとＢベクトルの角度を取得

    private:
        int x, y;
        double theta;
        double azimuth;
        #define M_PI 3.14159265358979323846 //保険
};