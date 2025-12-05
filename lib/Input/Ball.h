#pragma once

#include <Arduino.h>

class BALL {
    public:
        void setup(); //初期設定
        void read(); //センサーの読み取り
        bool get_stat(); //検出ステータスを取得（０＝検出失敗、１＝検出成功）
        int get_azimuth(); //方位角を取得
        int get_magnitude(); //距離を取得
        int get_x(); //X座標取得
        int get_y(); //Y座標取得
        int get_value(byte ballNUM); //指定したセンサーの強度を取得（引数は０～１５）

    private:
        bool ball; //検出ステータス
        int ball_x; //最終座標（処理済み）
        int ball_y; //最終座標（処理済み）
        int ball_x_; //RAW座標（そのまま）
        int ball_y_; //RAW座標（そのまま）
        int total_x = 0; //X座標合計値
        int total_y = 0; //Y座標合計値
        int ballvalues[16]; //センサー値を保存
        byte ballNUMstart; //グループ分け開始センサー番号
        byte max_ballNUM = 0; //最大強度を持つセンサー番号を保存
        int max_ballvalue = 0; //最大強度を保存
        int NUMball; //センサーの数を自動保存
        int history_x[3]; //X座標の履歴（LPF用）
        int history_y[3]; //Y座標の履歴（LPF用）
        int history_size; //履歴配列の大きさを自動保存
        const int ballPINs[16] = {31, 29, 27, 25, 23, 19, 17, 15, 32, 34, 36, 38, 40, 42, 44, 46}; //各センサーピン
        const float balldirs[16] = {0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5, 180, 202.5, 225, 247.5, 270, 292.5, 315, 337.5}; //各センサー角度
};