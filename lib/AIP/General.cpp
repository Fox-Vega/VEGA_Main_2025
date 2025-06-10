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
}

void General::startup() {
    mypixel.brightness(999);
    phase = 1;
    while (phase < 4) {
        tact_pressed = myswitch.check_tact();
        toggle_stat = myswitch.check_toggle();
        if (phase < 3) {
            mypixel.clear();
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
            if (ball.get_magnitude() != 0) {
                mypixel.closest(ball.get_azimuth(), 80, 0, 255, 1);
            }
            mypixel.show();
        }
        switch (phase) {
            case 1://選択フェーズ
                if (tact_pressed == 1){//アタック
                    mode = 1;
                    phase = 2;
                    mybuzzer.start(500, 200);
                } else if (tact_pressed == 2) {//ディフェンス
                    mode = 2;
                    phase = 2;
                    mybuzzer.start(500, 200);
                } else if (tact_pressed == 3) {//テスト
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
                    mybuzzer.start(100, 500);
                } else if (tact_pressed == 2) {
                    if (startcord < 4) {
                        startcord += 1;
                    } else {
                        startcord = 0;
                    }
                    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
                    mybuzzer.start(500, 50);
                    mybuzzer.start(500, 50);
                } else if (tact_pressed == 3) {//確定　次のフェーズへ
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
                } else if (tact_pressed == 2) { //方向と座標をリセット
                    gam.dir_reset();
                    mybuzzer.start(300, 500);
                } else if (tact_pressed == 3) {
                    //機能無し
                } else if (toggle_stat == 1) {//スタート！
                    phase = 4;
                } else {
                    if (mode == 3) {
                        mypixel.rainbow();
                        mypixel.show();
                    } else {
                        mypixel.rainbow();
                        mypixel.show();
                    }
                    if (millis() - lastbuzzer > 500) {
                        mybuzzer.start(400, 50);
                        lastbuzzer = millis();
                    }
                }
                break;
        }
    }
    mypixel.brightness(999);
    mypixel.clear();
    mypixel.show();
    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
}

int General::get_mode() {
    return mode;
}
