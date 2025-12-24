#include "General.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

int alliveCount = 0;
int remainingLife = 10000;
unsigned long deadtime = 0;


void General::setup() {
    Serial.begin(9600);
    myswitch.setup();
    mybuzzer.setup();
    mypixel.setup();
    mypixel.use_pixel(true);//常時つけてまあああああああああああああああああああああああああああああああす
    mypixel.brightness(15);
    mypixel.multi(0, 15, 255, 128,1);
    mypixel.show();
    Serial.println("Booting-----");
    ball.setup();
    Serial.println("ballFinish");
    line.setup();
    Serial.println("lineFinish");
    mymotor.setup();
    Serial.println("motorFinish");
    gam.setup();
    Serial.println("gamFinish");
    attack.setup();
    Serial.println("attackFinish");
    defense.setup();
    Serial.println("defenseFinish");
    mypixel.use_pixel(true);//常時つけてまあああああああああああああああああああああああああああああああす
    mypixel.multi(0, 15, 255, 255, 255);
    mypixel.show();
    if(myswitch.check_tact() == 15){
        mybuzzer.preset(0);
    } else {
        mybuzzer.preset(1);
    }
    standby = 0;
    Serial.println("GeneralSetupFinish");
}

int General::startup() {
    Serial.println("GeneralStartupStart");
    mymotor.free();
    mybuzzer.stop();
    mypixel.use_pixel(true);//常時つけてまあ
    mypixel.brightness(15);//低くしてまああああああああああああああああああああああああああああああああす

    if (standby == 0) {
        phase = 1;
        mode = 0;
        startcord = 0;
    } else {
        phase = 3;
    }
    alliveCount = 0;

    while (phase < 4) {
        gam.read_azimuth();
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
                mypixel.closest(ball.get_azimuth(), 80, 0, 255,1);
            }
        }
        switch (phase) {
            case 1://選択フェーズ
                alliveCount = 0;
                if (tact_pressed == 1){//アタック
                    mode = 1;
                    mybuzzer.start(500, 200);
                    phase = 2;
                } else if (tact_pressed == 5) {//ディフェンス
                    mode = 2;
                    mybuzzer.start(500, 200);
                    phase = 2;
                } else if (tact_pressed == 9) {//テスト
                    mode = 3;
                    phase = 2;
                    mybuzzer.start(500, 200);
                }
                delay(100);
                break;
            case 2://座標選択
                alliveCount = 0;
                if (tact_pressed == 1){//一つ戻る　選択フェーズへ
                    phase = 1;
                    startcord = 0;
                    mode = 0;
                    mybuzzer.start(100, 500);
                } else if (tact_pressed == 5) {
                    if (startcord < 4) {
                        startcord += 1;
                    } else {
                        startcord = 0;
                    }
                    mybuzzer.start(500, 50);
                    mybuzzer.start(500, 50);
                } else if (tact_pressed == 9) {//確定　次のフェーズへ
                    phase = 3;
                    mybuzzer.start(500, 500);
                    mypixel.clear();
                    mypixel.show();
                }
                break;
            case 3://トグルまち
                if (tact_pressed == 1){//一つ戻る　
                    phase = 2;
                    mybuzzer.start(100, 500);
                } else if (tact_pressed == 5) { //方向と座標をリセット
                    gam.dir_reset();
                    mybuzzer.start(300, 500);
                    alliveCount = 0;
                } else if (tact_pressed == 9) {
                    //機能無し
                    alliveCount = 0;
                    mypixel.rainbow();
                } else if (toggle_stat == 1) {//スタート！
                    standby = 1;
                    phase = 4;
                } else {
                    if(alliveCount>=remainingLife){
                        if(millis() - deadtime > 3000){
                            mybuzzer.stop();
                        } else if(millis() - lastbuzzer > 2930){
                            mypixel.multi(0, 15, 255, 255, 255);
                            mybuzzer.start(1800,999);
                        } else {
                            mypixel.multi(0, 15, 255, 0, 0);
                            mybuzzer.start(400, 999);
                        }
                        if(alliveCount==remainingLife)deadtime=millis();
                        alliveCount++;
                    } else if (millis() - lastbuzzer > 500) {
                        mypixel.rainbow();
                        alliveCount++;
                        mybuzzer.start(400, 50);
                        lastbuzzer = millis();
                    } else if (millis() - lastbuzzer > 320 && alliveCount > (remainingLife / 2+remainingLife/4)) {
                        mypixel.rainbow();
                    } else {
                        mypixel.rainbow();
                    }
                }
                break;
        }
        if(tact_pressed==15){
            if(mypixel.stat()==true){
                mypixel.use_pixel(0);
            } else{
                mypixel.use_pixel(1);
            }
            mybuzzer.start(800, 50);
            delay(500);
        }
        mypixel.shows();
    }
    //mypixel.brightness(100);
    mypixel.clear();
    mypixel.show();
    mymotor.stabilization(1);
    mymotor.move(1);
    defense.reset();
    return startcord;
}

int General::get_mode() {
    return mode;
}
