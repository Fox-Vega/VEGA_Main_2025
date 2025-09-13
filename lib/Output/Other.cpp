#include "Other.h"
#include "Input.h"
#include "Output.h"


void MyPIXEL::setup() {
    PIXEL.begin();
    PIXEL.setBrightness(PIXELbrightness);
    if (myswitch.check_tact() != 0) {
        usePIXEL = 1; //ネオピクセルの使用有無
        // delay(400);
    } else {
        usePIXEL = 0; //ネオピクセルの使用有無
    }
    mypixel.clear();
    PIXEL.show();
}

void MyPIXEL::brightness(int brightness) {
    if (brightness == 999) {
        PIXEL.setBrightness(PIXELbrightness);
    } else {
        PIXEL.setBrightness(brightness<0 ? 0 : (brightness>255 ? 255 : brightness));
    }
}

void MyPIXEL::uni(int PIXELNUM, int red, int green, int blue) {
    if (usePIXEL == 1) {
        PIXELNUM %= 16;
        PIXEL.setPixelColor(PIXELNUM, PIXEL.Color(red, green, blue));
    }
}

void MyPIXEL::multi(int PIXELNUMstart, int PIXELNUMend, int red, int green, int blue) {
    if (usePIXEL == 1) {
        for (int i = PIXELNUMstart; i <= PIXELNUMend; i++) {
            int a = i;
            a %= 16;
            mypixel.uni(a, red, green, blue);
        }
    }
}

void MyPIXEL::closest(int azimuth, int red, int green, int blue, int num) {
    if (usePIXEL == 1) {
        float ClosestPIXEL = (azimuth / 360.0f * NUMPIXEL); //浮動小数点演算を明確化
        ClosestPIXEL = round(ClosestPIXEL); //標準の丸め関数を使用

        //ピクセルの範囲補正
        if (ClosestPIXEL >= NUMPIXEL) {
            ClosestPIXEL = 0;
        }

        //numが偶数の場合も正しく範囲を決定
        PIXELNUMstart = ClosestPIXEL - (num / 2);
        if (PIXELNUMstart < 0) {
            PIXELNUMstart += NUMPIXEL;
        }

        for (int i = 0; i < num; i++) {
            int j = (PIXELNUMstart + i) % 16; //インデックス補正を動的に処理

            mypixel.uni(j, red, green, blue);
        }
    }
}

void MyPIXEL::rainbow() {
    if (usePIXEL == 1) {
        for(int i = 0; i < (int)PIXEL.numPixels(); i++) {
            //ストリップの長さに沿って色相環（65536の範囲）を1回転させる量だけピクセルの色相をオフセットします。
            int pixelHue = step_num + (i * 65536L / PIXEL.numPixels());
            //ColorHSV関数に色相(0 to 65535)を渡し、その結果をgamma32()でガンマ補正します。
            PIXEL.setPixelColor(i, PIXEL.gamma32(PIXEL.ColorHSV(pixelHue)));
        }
        step_num += 256;
        if (step_num == 65536) {
            step_num = 0;
        }
        delay(8);
    }
}

void MyPIXEL::show() {
    if (usePIXEL == 1) {
        PIXEL.show();
    }
}

void MyPIXEL::shows() {
    PIXEL.show();
}

void MyPIXEL::clear() {
    if (usePIXEL == 1) {
        for (int i = 0; i < NUMPIXEL; i++) {
            PIXEL.setPixelColor(i, PIXEL.Color(0, 0, 0));
        }
    }
}

void MyPIXEL::clears() {
    for (int i = 0; i < NUMPIXEL; i++) {
        PIXEL.setPixelColor(i, PIXEL.Color(0, 0, 0));
    }
}

void MyPIXEL::use_pixel(bool stat) {
    usePIXEL = stat;
}

bool MyPIXEL::stat() {
    return usePIXEL;
}

void MyBUZZER::setup() {
    pinMode(BUZZER_PIN, OUTPUT);
}

void MyBUZZER::start(int BUZZERnote, int BUZZERduration) {
    if (useBUZZER == 1) {
        if (BUZZERduration != 999) {
            tone(BUZZER_PIN, BUZZERnote, BUZZERduration);
            delay(BUZZERduration * 1.5);
            noTone(BUZZER_PIN);
            delay(10);
        } else {
            tone(BUZZER_PIN, BUZZERnote);
            delay(10);
        }
    }
}

void MyBUZZER::stop() {
    noTone(BUZZER_PIN);
}

void MyBUZZER::preset(int BUZZERpresetNUM) {
    if (useBUZZER == 1) {
        if (BUZZERpresetNUM == 0) {
            int Melody_Preset1[] = {260, 260, 260, 255, 0, 300, 250, 330};
            int NoteDurs[] = {7, 7, 7, 7, 14, 4, 5, 3};
            int Size_Melody_Preset1 = sizeof(Melody_Preset1)/sizeof(Melody_Preset1[0]);
            for (int playing_Note = 0; playing_Note < Size_Melody_Preset1; playing_Note++) {
                    int noteDur = 1000 / NoteDurs[playing_Note];
                    tone(BUZZER_PIN, Melody_Preset1[playing_Note], noteDur);
                    int PBTWNotes = noteDur * 1.4;
                    delay(PBTWNotes);
                    noTone(BUZZER_PIN);
            }
            delay(100);
        } else if (BUZZERpresetNUM == 1) {
            int Melody_Preset1[] = {350, 360, 380};
            int NoteDurs[] = {8, 8, 8};
            int Size_Melody_Preset1 = sizeof(Melody_Preset1)/sizeof(Melody_Preset1[0]);
            for (int playing_Note = 0; playing_Note < Size_Melody_Preset1; playing_Note++) {
                    int noteDur = 1000 / NoteDurs[playing_Note];
                    tone(BUZZER_PIN, Melody_Preset1[playing_Note], noteDur);
                    int PBTWNotes = noteDur * 1.4;
                    delay(PBTWNotes);
                    noTone(BUZZER_PIN);
            }
            delay(100);
        }
    }
}
