#pragma once

#include <Arduino.h>

class MyVECTOR {
    public:
        void get_cord(int azimuth, int magnitude);//座標を計算する関数
        void get_plpocord(int po_x, int po_y); //player to positionの座標を計算する関数　コート基準座標を入力　実行前に座標更新
        void get_tarcord(int tar_azimuth, int tar_magnitude); //targetのコート座標を計算する関数　実行前に座標更新
        void get_svec(int tar_azimuth, int tar_magnitude); //targetの速度ベクトルを計算する関数　コート基準座標を入力　実行前に座標更新
        int get_azimuth(int x, int y); //方位角を計算する関数
        int get_magnitude(int x, int y); //距離を計算する関数
        int get_x(); //取得したx座標を返す関数
        int get_y(); //取得したy座標を返す関数
        int get_plpo_x(); //posの自機基準x座標を返す関数
        int get_plpo_y(); //posの自機基準y座標を返す関数
        int get_tar_x(); //targetのコート基準x座標を返す関数
        int get_tar_y(); //targetのコート基準y座標を返す関数
        int get_tarsvec_x(); //targetのコート基準速度ベクトルのx座標を返す関数
        int get_tarsvec_y(); //targetのコート基準速度ベクトルのy座標を返す関数
        int get_tarsvecazimuth(); //targetのコート基準速度ベクトルの方位角を返す関数
        int get_tarsvecmagnitude(); //targetのコート基準速度ベクトルの方位角を返す関数

    private:
        int x, y;

        //速度ベクトル系
        int cord_x, cord_y;
        int tarcord_x, tarcord_y;
        int lastcord_x, lastcord_y;
        int lasttar_x, lasttar_y;
        int svec_x, svec_y;
        int tarsvec_x, tarsvec_y;
        int tarsvec_magnitude;
        int tarsvec_azimuth;

        int plpo_x, plpo_y;
        float theta;
        float azimuth;
        float magnitude;
        unsigned long lastupdatetime;
        unsigned long dt;

        int a = (int)1.1;
};
