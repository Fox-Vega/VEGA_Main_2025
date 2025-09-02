#include "General.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"


void General::setup() {
    Serial.begin(9600);
    myswitch.setup();
    mybuzzer.setup();
    mypixel.setup();
    mypixel.multi(0, 15, 255, 128,1, 0);
    mypixel.show();

    ball.setup();
    Serial.println("1");
    line.setup();
    Serial.println("2");
    mymotor.setup();
    Serial.println("3");
    gam.setup();
    mypixel.multi(0, 15, 255, 255, 255,1);
    mypixel.show();
    if(myswitch.check_tact() == 15){
        mybuzzer.preset(0);
    } else {
        mybuzzer.preset(1);
    }
    standby = 0;
}

void General::startup() {
    mymotor.free();
    mybuzzer.stop();
    // mypixel.brightness(999);

    if (standby == 0) {
        phase = 1;
        mode = 0;
    } else {
        phase = 3;
    }


    while (phase < 4) {
        gam.read_azimuth();
        mypixel.clears();
        tact_pressed = myswitch.check_tact();
        toggle_stat = myswitch.check_toggle();
        if (phase < 3) {
            if (mode == 1) {
                mypixel.multi(0, 15, 255, 130, 130,1);//アタック
            } else if (mode == 2) {
                mypixel.multi(0, 15, 130, 255, 130,1);//ディフェンス
            } else if (mode == 3) {
                mypixel.multi(0, 15, 255, 255, 90,1);//テスト
            }
            int d = 0 - gam.get_azimuth();
            if (d < 0) {
                d += 360;
            }
            mypixel.closest(d, 255, 0, 100,1, 3);
            if (startPIXELs[startcord] == 99) {
                mypixel.uni(2, 255, 255, 255,1);
                mypixel.uni(6, 255, 255, 255,1);
                mypixel.uni(10, 255, 255, 255,1);
                mypixel.uni(14, 255, 255, 255,1);
            } else {
                mypixel.uni(startPIXELs[startcord], 255, 255, 255,1);
            }
            ball.read();
            if (ball.get_value(99) != 0) {
                mypixel.closest(ball.get_azimuth(), 80, 0, 255,1, 1);
            }
        }
        switch (phase) {
            case 1://選択フェーズ
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
            case 2:
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
                    if (mode == 3||mode ==2) {
                        mypixel.rainbow();
                    }
                    if (millis() - lastbuzzer > 500) {
                        mybuzzer.start(400, 50);
                        lastbuzzer = millis();
                    }
                }
                break;
        }
        if(tact_pressed==15){
            if(mypixel.pixelEnabled()==true){
                mypixel.use_pixel(0);
            } else{
                mypixel.use_pixel(1);
            }
            mybuzzer.start(800, 50);
        }
        mypixel.shows();
    }
    //mypixel.brightness(100);
    mypixel.clear();
    mypixel.show();
    gam.cord_custom(startcords_x[startcord], startcords_y[startcord]);
    mymotor.stabilization(1);
    mymotor.move(1);
}
inline void General::readCommand(){//シリアル使ってるけど送信しなきゃ速度は食わない　食うなら作らない定期
    if (Serial.available()) {
        String line = Serial.readStringUntil('\n');
        line.trim();
        if(line.length() > 0 && line[0] == '/') {//コマンドを検知
            String parts[5];
            int count = 0;
            int start = 0;

            for (int i = 0; i <= (int)line.length() && count < 5; i++) {  // キャスト追加
                if (i == (int)line.length() || line[i] == ' ') {
                    if (i > start) {
                        parts[count] = line.substring(start, i);
                        count++;
                    }
                    start = i + 1;
                }
            }

            // for (int i = 0; i < count; i++) {
            //     Serial.print("part"); Serial.print(i);
            //     Serial.print("="); Serial.println(parts[i]);
            // }

            //実行ゾーン
            String command = parts[0].substring(1); // 最初の文字(/)を除去
            if(myswitch.check_toggle()==0){
                if(command == "mode"){
                    if(parts[1]=="attack"||parts[1]=="0"||parts[1]=="Attack"||parts[1]=="ATTACK"){
                        if(parts[2]==""||parts[2]=="select"){
                            mode = 1;
                            mybuzzer.start(500, 200);
                            phase = 2;
                            Serial.println("Attack mode selected");
                        } else if(parts[2]=="run"){
                            mode = 1;
                            mybuzzer.start(500, 200);
                            delay(10);
                            mybuzzer.start(500, 500);
                            phase = 3;
                            Serial.println("Attack mode run");
                        } else {
                            Serial.println("Invalid sub command. Use select or run.");
                        }
                    } else if(parts[1]=="defense"||parts[1]=="1"||parts[1]=="Defense"||parts[1]=="DEFENSE"||parts[1]=="Defence"||parts[1]=="DEFENCE"||parts[1]=="Defence"){
                        if(parts[2]==""||parts[2]=="select"){
                            mode = 2;
                            mybuzzer.start(500, 200);
                            phase = 2;
                            Serial.println("Defense mode selected");
                        } else if(parts[2]=="run"){
                            mode = 2;
                            mybuzzer.start(500, 200);
                            delay(10);
                            mybuzzer.start(500, 500);
                            phase = 3;
                            Serial.println("Defense mode run");
                        } else {
                            Serial.println("Invalid sub command. Use select or run.");
                        }
                    } else if(parts[1]=="test"||parts[1]=="2"||parts[1]=="Test"||parts[1]=="TEST"){
                        if(parts[2]==""||parts[2]=="select"){
                            mode = 3;
                            mybuzzer.start(500, 200);
                            phase = 2;
                            Serial.println("Test mode selected");
                        } else if(parts[2]=="run"){
                            mode = 3;
                            mybuzzer.start(500, 200);
                            delay(10);
                            mybuzzer.start(500, 500);
                            phase = 3;
                            Serial.println("Test mode run");
                        } else {
                            Serial.println("Invalid sub command. Use select or run.");
                        }
                    } else {
                        Serial.println("Invalid mode. Use attack, defense, or test.");
                    }
                } else if(command == "help"){
                    Serial.println("Available commands:");
                } else if (command == "pixel"){
                    if(parts[1] == "on"||parts[1] == "true"){
                        mypixel.use_pixel(1);
                        mybuzzer.start(800, 50);
                        Serial.println("Turning pixel on");
                    } else if(parts[1] == "off"||parts[1] == "false"){
                        mypixel.use_pixel(0);
                        mybuzzer.start(800, 50);
                        Serial.println("Turning pixel off");
                    } else if(parts[1]=="brightness"||parts[1]=="br"){
                        mypixel.brightness(parts[2].toInt());
                        mybuzzer.start(1000, 50);
                        Serial.print("Setting pixel brightness to ");
                        Serial.println(parts[2]);
                    } else {
                        Serial.println("Invalid pixel command. Use true or false.");
                    }
                } else if(command =="phase"){
                    if(parts[1]="set"){
                    if(parts[2] == "1"){
                        Serial.println("Setting phase to 1");
                        phase = 1;
                        mybuzzer.start(800, 50);
                    } else if(parts[2] == "2"){
                        Serial.println("Setting phase to 2");
                        phase = 2;
                        mybuzzer.start(800, 50);
                    } else if(parts[2] == "3"){
                        Serial.println("Setting phase to 3");
                        phase = 3;
                        mybuzzer.start(800, 50);
                    } }
                    else if(parts[1] == "next"){
                        Serial.println("Setting phase to next");
                        if(phase < 3) {
                            phase += 1;
                            mybuzzer.start(500, 100);
                        } else {
                            Serial.println("Already at the last phase.");
                        }
                    } else if(parts[1] == "back"){
                        Serial.println("Setting phase to back");
                        if(phase > 1) {
                            phase -= 1;
                            mybuzzer.start(100, 100);
                        } else {
                            Serial.println("Already at the first phase.");
                        }
                    } else if(parts[1]== "get"){
                        Serial.print("Current phase: ");
                        Serial.println(phase);
                    } else {
                        Serial.println("Invalid phase. Use 1, 2, 3, or next, back.");
                    }
                } else if(command=="get"){
                    if(parts[1]=="phase"){
                        Serial.print("Current phase: ");
                        Serial.println(phase);
                    } else if(parts[1]=="mode"){
                        Serial.print("Current mode: ");
                        Serial.println(mode);
                    } else if(parts[1]=="ball"){
                        ball.read();
                        Serial.print("Ball azimuth:"+String(ball.get_azimuth())+" ball distance:"+String(ball.get_magnitude()));
                    } else if(parts[1]=="line"){
                        Serial.println("err can't read line sensor not found line.read()");
                    } else{
                        Serial.println("Invalid get command.");
                    }
                } else if(command=="gam"){
                    if(parts[1]=="reset"){
                        Serial.println("Resetting azimuth");
                        gam.dir_reset();
                        mybuzzer.start(300, 500);
                    }
                } else if(command=="defense"||command=="1"||command=="Defense"||command=="DEFENSE"||command=="Defence"||command=="DEFENCE"||command=="Defence"){
                    if(parts[1]=="Serial"){
                        if(parts[2]=="true"||parts[2]=="on"){
                            Serial.println("defense:Uses Serial set to "+String(defense.useSerial(true)));
                        } else if(parts[2]=="false"||parts[2]=="off"){
                            Serial.println("defense:Uses Serial set to "+String(defense.useSerial(false)));
                        } else {
                            Serial.println("Invalid serial command. Use true or false.");
                        }
                    } else {
                        Serial.println("Invalid defense command.");
                    }
                } else {
                    Serial.print("Unknown command: ");
                    Serial.println(parts[1]+String(" was not found in library."));
                }
            } else {
                Serial.println("Before use Serial command,toggle switch must be far side.");//トグルが奥じゃないと動作しないように
            }
        }
    }
}
int General::get_mode() {
    return mode;
}