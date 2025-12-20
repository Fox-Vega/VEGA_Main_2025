#pragma once

#include <Arduino.h>
#include <math.h>

class LINE {
    public:
        void setup(); //初期設定
        void read(); //センサーの読み取り
        int get_azimuth(); //方位角を取得
        int get_magnitude(); //距離を取得
        int get_x(); //X座標取得
        int get_y(); //Y座標取得
        int get_type(); //検出ステータスを取得（反応グループの数）
        int get_eazimuth(); //逃げる方向を取得
        int get_value(byte lineNUM); //指定したセンサー（０～２３）の強度を取得（引数は０～１５）
        bool get_stat(byte lineNUM); //指定したセンサー（０～２３）の検出ステータスを取得（０＝検出失敗、１＝検出成功）
        int get_pack(byte packNUM); //指定した反応グループの角度を取得（引数はget_typeで反応した数の範囲　２なら０～１）

    private:
        const int detection_border = 990; //700＠部室　990＠草塩
        const int over_border = 120; //ライン越え判定を行う変化量

        float total_x; //X座標合計値
        float total_y; //Y座標合計値

        int line_x; //最終座標（処理済み）
        int line_y; //最終座標（処理済み）
        int oldline_x; //前回の最終座標（処理済み）
        int oldline_y; //前回の最終座標（処理済み）
        int escape_x; //最終逃げる座標（処理済み）
        int escape_y; //最終逃げる座標（処理済み）

        int line_type;
        //０＝反応なし
        //１＝点検知
        //２＝辺検知
        //３＝角検知

        bool over = false; //ライン越え判定

        const int line_r = 12; //ラインセンサーの半径
        const uint8_t selectPIN[3] = {22, 24, 26}; //選択ピン
        const uint8_t outputPIN[3] = {A9, A11, A13}; //取得ピン

        float pack_x[4]; //反応グループのX座標
        float pack_y[4]; //反応グループのY座標
        int line_values[24]; //センサー値（そのまま）
        int line_stat[24]; //最終ステータス
        int line_stat_[24]; //ステータス

        const byte Reader[8][3] = {
            {1, 1, 1},
            {1, 1, 0},
            {1, 0, 1},
            {1, 0, 0},
            {0, 1, 1},
            {0, 1, 0},
            {0, 0, 1},
            {0, 0, 0},
        };
        const int line_degs[24] = {15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345, 0}; //各センサー角度
};