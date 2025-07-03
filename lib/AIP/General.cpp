#include "General.h"
#include "Input.h"
#include "Output.h"


void General::setup() {
    Serial.begin(9600);
    myswitch.setup();
    mybuzzer.setup();
    mypixel.setup();
    mypixel.multi(0, 15, 255, 128, 0);
    mypixel.show();
    ball.setup();
    line.setup();
    mymotor.setup();
    gam.setup();
    mypixel.multi(0, 15, 255, 255, 255);
    mypixel.show();
    mybuzzer.preset(1);
    standby = 0;
}

void General::startup() {
    mymotor.free();
    mybuzzer.stop();
    mypixel.brightness(999);
    mypixel.use_pixel(0);

    if (standby == 0) {
        phase = 1;
        mode = 0;
    } else {
        phase = 3;
    }


    while (phase < 4) {
        mypixel.clears();
        tact_pressed = myswitch.check_tact();
        toggle_stat = myswitch.check_toggle();
        if (phase < 3) {
            if (mode == 1) {
                mypixel.multi(0, 15, 255, 130, 130);//アタック
            } else if (mode == 2) {
                mypixel.multi(0, 15, 130, 255, 130);//ディフェンス
            } else if (mode == 3) {
                mypixel.multi(0, 15, 255, 255, 90);//テスト
            }
            int d = 0 - gam.get_azimuth();
            if (d < 0) {
                d += 360;
            }
            mypixel.closest(d, 255, 0, 100, 3);
            if (startPIXELs[startcord] == 99) {
                mypixel.uni(2, 255, 255, 255);
                mypixel.uni(6, 255, 255, 255);
                mypixel.uni(10, 255, 255, 255);
                mypixel.uni(14, 255, 255, 255);
            } else {
                mypixel.uni(startPIXELs[startcord], 255, 255, 255);
            }
            ball.read();
            if (ball.get_value(99) != 0) {
                mypixel.closest(ball.get_azimuth(), 80, 0, 255, 1);
            }
        }
        switch (phase) {
            case 1://選択フェーズ
                if (tact_pressed == 1){//アタック
                    mypixel.use_pixel(0);
                    mode = 1;
                    mybuzzer.start(500, 200);
                    phase = 2;
                } else if (tact_pressed == 5) {//ディフェンス
                    mypixel.use_pixel(0);
                    mode = 2;
                    mybuzzer.start(500, 200);
                    phase = 2;
                } else if (tact_pressed == 9) {//テスト
                    mypixel.use_pixel(1);
                    mode = 3;
                    phase = 2;
                    mybuzzer.start(500, 200);
                }
                delay(100);
                break;
            case 2:
                if (tact_pressed == 1){//一つ戻る　選択フェーズへ
                    phase = 1;
                    startcord = 0;
                    mode = 0;
                    mypixel.use_pixel(0);
                    mybuzzer.start(100, 500);
                } else if (tact_pressed == 5) {
                    if (startcord < 4) {
                        startcord += 1;
                    } else {
                        startcord = 0;
                    }
                    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
                    mybuzzer.start(500, 50);
                    mybuzzer.start(500, 50);
                } else if (tact_pressed == 9) {//確定　次のフェーズへ
                    phase = 3;
                    mybuzzer.start(500, 500);
                    mypixel.clear();
                    mypixel.show();
                }
                break;
            case 3:
                if (tact_pressed == 1){//一つ戻る　
                    phase = 2;
                    mybuzzer.start(100, 500);
                } else if (tact_pressed == 5) { //方向と座標をリセット
                    gam.dir_reset();
                    mybuzzer.start(300, 500);
                } else if (tact_pressed == 9) {
                    //機能無し
                } else if (toggle_stat == 1) {//スタート！
                    standby = 1;
                    phase = 4;
                } else {
                    if (mode == 3) {
                        mypixel.rainbow();
                    }
                    if (millis() - lastbuzzer > 500) {
                        mybuzzer.start(400, 50);
                        lastbuzzer = millis();
                    }
                }
                break;
        }
        mypixel.shows();
    }
    mypixel.brightness(100);
    mypixel.clear();
    mypixel.show();
    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
    mymotor.stabilization(1);
    mymotor.move(1);
}

int General::get_mode() {
    return mode;
}