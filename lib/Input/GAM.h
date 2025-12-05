#pragma once

#include <Arduino.h>

class GAM { //いろいろエラーが出てたから弄った結果、GAMクラス誕生☆　Gyro,Accelometer,Magneticをテキトーにもじりました
    public:
        void setup(); //初期設定
        void read_azimuth(); //ジャイロの読み取り
        int get_azimuth(); //方位角の取得
        void dir_reset(); //方向の初期化

    private:
        int azimuth;
        int yawtweak;
};
