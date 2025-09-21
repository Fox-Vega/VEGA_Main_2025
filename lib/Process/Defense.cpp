#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

// フィルタされたボール角度（static変数）
static int ball_azimuth = 0;


// ===================================
// メイン処理
// ===================================

void Defense::setup() {
    line_r = line.get_R();
}

void Defense::defense_() {
    frog=0;
    if(SilentTime.read_milli()>dash_border&& USE_DASH == true){
        SilentTime.reset();
        while(SilentTime.read_milli()<100){
            mymotor.run(0,100,0);
        }
        SilentTime.reset();
        mypixel.multi(0,15,255,255,255);
        mypixel.show();
        while(SilentTime.read_milli()<dash_time){
            ball.read();
            line.read();
            if(line.get_type()!=0){lastdetect=line.get_azimuth();}
            mymotor.run(ball.get_azimuth(),200,0);
        }
        SilentTime.reset();
    }
    else{
    if (line.get_type() == 0) {
        // === ラインなし：戻り処理 ===
        mymotor.run(lastdetect, 180, 0);
        mybuzzer.start(1500, 999);
        frog=2;

    } else {
        frog=1;
        // === ラインあり：メイン処理 ===

        // --- 次回用保存処理 ---
        mybuzzer.stop();
        lastdetect = line.get_azimuth();

        if (ball.get_stat() == 1) {
            // === ボールあり ===

            // --- ボール(X軸)処理 ---
            move_x = -ball_power;
            if (ball.get_azimuth() < 180) {
                move_x = ball_power;
            }

            ball_filter();
            if (getErr(ball_get_Aazimuth(), 0) < ball_move_border) {
                move_x = 0;
            }

            // --- ライン(Y軸)処理 ---
            move_y = ((line_max / line_r) * line.get_y()) * line_late;

            // --- 角処理 ---
            if (abs(line.get_x()) > 3) {
                frog=4;
                ex=false;
                if (diff_signs(line.get_x(), move_x)) {
                    // X軸はライン優先
                    ex=true;
                    move_x = ((line_max / line_r) * line.get_x()) * line_late;
                }

                if (diff_signs(line.get_x(), ball.get_x())) {
                    // ボール距離が大きくかつラインに近いなら賭ける
                    if (ball.get_magnitude() > exitCorner && line.get_magnitude() < (line_max / 3)) {
                        frog=5;
                        move_x = -ball_power;
                        if (ball.get_azimuth() < 180) {
                            move_x = ball_power;
                        }
                        move_y = ball_power;
                        if (ball.get_y() < 0) {
                            move_y = -ball_power;
                        }
                    }
                }
            }

            // --- 移動量計算 ---
            move_azimuth = myvector.get_azimuth(move_x, move_y);
            move_power = myvector.get_magnitude(abs(move_x), move_y);

            // --- 移動実行 ---
            if (move_power > move_border) {
                mymotor.run(move_azimuth, (int)move_power, 0);
            } else {
                frog=6;
                mymotor.free();
            }
            Dtimer.reset();

        } else {
            frog=3;
            // === ボールなし ===
            move_x = 0;
            move_y = (line.get_y() * 20) * line_late;
            move_azimuth = myvector.get_azimuth(move_x, move_y);
            move_power = myvector.get_magnitude(abs(move_x), move_y);

            if (move_power > move_border) {
                mymotor.run(move_azimuth, (int)move_power, 0);
            } else {
                mymotor.free();
            }
            Dtimer.reset();
        }
    }
    applyUI(frog);
}
    if(frog!=6){SilentTime.reset();}
}

void Defense::reset() {
    resetUI();
    Dtimer.reset();
    for (int i = 0; i < 5; i++) {
        ball_history[i] = ball.get_azimuth();
    }
}

// ===================================
// ボールフィルタ処理
// ===================================

int Defense::ball_get_Aazimuth() {
    #ifdef BALL_FILTER_OFF
    return ball.get_azimuth(); // フィルタをオフにする場合
    #endif
    return ball_azimuth; // フィルタされた角度を返すだけ
}

void Defense::ball_filter() {
    // --- 履歴配列に新しいデータを追加 ---
    for (int i = 4; i > 0; i--) {
        ball_history[i] = ball_history[i - 1];
    }
    ball_history[0] = ball.get_azimuth();

    // --- テンプ配列にコピーして中央値を計算 ---
    int tempArr[5];
    for (int i = 0; i < 5; i++) {
        tempArr[i] = ball_history[i];
    }

    // --- 基準角度を最初の値にする ---
    int base = tempArr[0];

    // --- 基準角度からの差分に変換（-180～+180の範囲に正規化） ---
    for (int i = 0; i < 5; i++) {
        int diff = tempArr[i] - base;
        if (diff > 180) diff -= 360;
        if (diff < -180) diff += 360;
        tempArr[i] = diff;
    }

    // --- 差分をソート ---
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4 - i; j++) {
            if (tempArr[j] > tempArr[j + 1]) {
                int temp = tempArr[j];
                tempArr[j] = tempArr[j + 1];
                tempArr[j + 1] = temp;
            }
        }
    }

    // --- 中央値を計算 ---
    int median = base + tempArr[2];
    if (median < 0) median += 360;
    if (median >= 360) median -= 360;

    // --- 現在値と中央値を比較 ---
    int current = ball.get_azimuth();
    int diff = abs(getErr(current, median));

    // --- 差が大きい場合（外れ値）は中央値を使用、そうでなければ現在値 ---
    if (diff > ballFilter) {
        ball_azimuth = median;
    } else {
        ball_azimuth = current;
    }
    frog=frog;
}

// ===================================
// UI処理
// ===================================

void Defense::resetUI() {
    // --- 背景 #39C5BB7F ---
    background.red = 57;
    background.green = 197;
    background.blue = 187;
    background.alpha = 0.5;

    // --- ライン #00C832BF ---
    P_line.red = 0;
    P_line.green = 200;
    P_line.blue = 50;
    P_line.alpha = 0.75;

    // --- ボール #FF0000BF ---
    P_ball.red = 255;
    P_ball.green = 0;
    P_ball.blue = 0;
    P_ball.alpha = 0.75;

    // --- 移動方向 #0000FFFF ---
    move_ang.red = 0;
    move_ang.green = 0;
    move_ang.blue = 255;
    move_ang.alpha = 0.75;

    // --- ダッシュタイマー #DCB4DCFF ---
    dash_timer.red = 220;
    dash_timer.green = 180;
    dash_timer.blue = 220;
    dash_timer.alpha = 1.0;

    // --- 追加移動X軸 #8bfff7ff ---
    exMoveX.red = 255;
    exMoveX.green = 255;
    exMoveX.blue = 0;
    exMoveX.alpha = 1.0;

    // --- 追加移動Y軸 #d0ff50ff ---
    exMoveY.red = 208;
    exMoveY.green = 255;
    exMoveY.blue = 80;
    exMoveY.alpha = 1.0;
}

void Defense::applyUI(int mode) {
    if(mode ==1){ //ノーマル
    // --- 背景 ---
    mypixel.multi(0, 15,
        background.red * background.alpha,
        background.green * background.alpha,
        background.blue * background.alpha);

    // --- ライン ---
    mypixel.closest(line.get_azimuth(),
        P_line.red * P_line.alpha,
        P_line.green * P_line.alpha,
        P_line.blue * P_line.alpha, 3);

    // --- ボール ---
    if (ball.get_stat() != 0) {
        mypixel.closest(ball.get_azimuth(),
            P_ball.red * P_ball.alpha,
            P_ball.green * P_ball.alpha,
            P_ball.blue * P_ball.alpha, 1);
    }

    // --- 移動方向 ---
    if(move_power>move_border){
    mypixel.closest(move_azimuth,
            move_ang.red * move_ang.alpha,
            move_ang.green * move_ang.alpha,
            move_ang.blue * move_ang.alpha, 2);
    }
} else if(mode==2){ //ラインなし
    // --- 背景 ---
    mypixel.multi(0, 15,
        255,
        0,
        0);

    // --- lastライン ---
    mypixel.closest(lastdetect,
        P_line.red,
        P_line.green,
        P_line.blue, 3);
} else if(mode==3){
    mypixel.multi(0, 15,
        255,
        255,
        255);
} else if(mode==4){ //角
    // --- 背景 ---
    mypixel.multi(0, 15,
        background.red * background.alpha,
        background.green * background.alpha,
        background.blue * background.alpha);

    // --- ライン ---
    mypixel.closest(line.get_azimuth(),
        P_line.red,
        P_line.green,
        P_line.blue, 3);

    // --- ボール ---
    if (ball.get_stat() != 0) {
        mypixel.closest(ball.get_azimuth(),
            P_ball.red,
            P_ball.green,
            P_ball.blue, 1);
    }

    int d=ball.get_azimuth()<180?90:270;
    // --- 移動方向 ---
    if(move_power>move_border){
    mypixel.closest(d,
            move_ang.red,
            move_ang.green,
            move_ang.blue, 2);
    }

    // --- 追加移動X軸 ---
    if(ex){
    mypixel.closest(norm360(d+180),
        exMoveX.red,
        exMoveX.green,
        exMoveX.blue, 2);
    }
} else if(mode==5){ //角(賭け)
    // --- 背景 ---
    mypixel.multi(0, 15,
        background.red * background.alpha,
        background.green * background.alpha,
        background.blue * background.alpha);
    int d=ball.get_azimuth();
    // --- 移動方向 ---
    if(move_power>move_border){
    mypixel.closest(d,
            exMoveY.red,
            exMoveY.green,
            exMoveY.blue, 2);
    }
} else if(mode==6){ //移動量（サイレントタイマーメーター）
    // --- 背景 ---
    mypixel.multi(0, 15,
        background.red * background.alpha,
        background.green * background.alpha,
        background.blue * background.alpha);

    // --- サイレントタイマーのメーター表示 ---
    // dash_border(8000ms)のうち何％か
    float percent = (float)SilentTime.read_milli() / (float)dash_border;
    if(percent > 1.0) percent = 1.0;

    // 16ピクセル中何個光らせるか
    int num_pixels = (int)(percent * 16);

    if(num_pixels > 0){
        mypixel.multi(0, num_pixels-1, 
            dash_timer.red * dash_timer.alpha, 
            dash_timer.green * dash_timer.alpha, 
            dash_timer.blue * dash_timer.alpha); // 0～num_pixels-1まで点灯
    }
}
}