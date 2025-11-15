#pragma once

#include <Arduino.h>

class MyVECTOR {
    public:
        /// @brief ベクトルの方位角を取得する
        /// @param x X成分
        /// @param y Y成分
        /// @return 方位角（度）
        float get_azimuth(float x, float y);
        /// @brief ベクトルの大きさを取得する
        /// @param x X成分
        /// @param y Y成分
        /// @return 大きさ
        float get_magnitude(float x, float y);
        /// @brief ベクトルの座標を計算する
        /// @param azimuth 方位角
        /// @param magnitude 大きさ
        void get_cord(float azimuth, float magnitude);
        /// @brief 計算されたベクトルのX座標を取得する
        /// @return X座標
        float get_x();
        /// @brief 計算されたベクトルのY座標を取得する
        /// @return Y座標
        float get_y();
        /// @brief 2つのベクトル間の角度を取得する
        /// @param ax ベクトルAのX成分
        /// @param ay ベクトルAのY成分
        /// @param bx ベクトルBのX成分
        /// @param by ベクトルBのY成分
        /// @return 角度（度）
        float get_vectordegree(float ax, float ay, float bx, float by);

    private:
        int x, y;
        double theta;
        double azimuth;
        #define M_PI 3.14159265358979323846 //保険
};