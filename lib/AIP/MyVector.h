#pragma once

#include <Arduino.h>

class MyVECTOR {
    public:
        void get_cord(int azimuth, int magnitude);//座標を計算する関数
        void get_plpocord(int po_x, int po_y); //player to positionの座標を計算する関数　引数にはコート基準座標を入力　実行前に座標更新して
        void get_tarcord(int tar_azimuth, int tar_magnitude); //targetのコート座標を計算する関数　引数には自機基準の座標入力　実行前に座標更新
        void get_svec(int tar_azimuth, int tar_magnitude, int memoryNUM); //targetの速度ベクトルを計算する関数　自機基準座標を入力　実行前に座標更新　ボールの追跡にはメモリ0を使用
        int get_azimuth(int x, int y); //方位角を計算する関数
        int get_magnitude(int x, int y); //距離を計算する関数
        int get_x(); //取得したx座標を返す関数
        int get_y(); //取得したy座標を返す関数

    private:
        int x, y;

        int cord_x, cord_y;
        int tarcord_x, tarcord_y;
        int old_cord_x[1], old_cord_y[1];
        int old_tar_x[1], oldtar_y[1];
        int svec_x, svec_y;
        int tarsvec_x, tarsvec_y;
        int plpo_x, plpo_y;
        float theta;
        float azimuth;
        float magnitude;
        unsigned long lastupdatetime;
        unsigned long dt;

        int a = (int)1.1;
};
