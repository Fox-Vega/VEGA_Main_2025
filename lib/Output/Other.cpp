#include "Other.h"
#include "Output.h"


void MyPIXEL::setup() {
        PIXEL.begin();
        PIXEL.setBrightness(PIXELbrightness);
}

void MyPIXEL::brightness(int brightness) {
    if (brightness == 999) {
        PIXEL.setBrightness(PIXELbrightness);
    } else {
        PIXEL.setBrightness(brightness);
    }
}

void MyPIXEL::uni(int PIXELNUM, int red, int green, int blue) {
    if (usePIXEL == true) {
        PIXEL.setPixelColor(PIXELNUM, PIXEL.Color(red, green, blue));
        PIXEL.show();
    }
}

void MyPIXEL::multi(int PIXELNUMstart, int PIXELNUMend, int red, int green, int blue) {
    if (usePIXEL == true) {
        for (int i = PIXELNUMstart; i <= PIXELNUMend; i++) {
            mypixel.uni(i, red, green, blue);
        }
    }
}

void MyPIXEL::unis(int PIXELNUM, int red, int green, int blue) {
    PIXEL.setPixelColor(PIXELNUM, PIXEL.Color(red, green, blue));
    PIXEL.show();
}

void MyPIXEL::multis(int PIXELNUMstart, int PIXELNUMend, int red, int green, int blue) {
    for (int i = PIXELNUMstart; i <= PIXELNUMend; i++) {
        mypixel.uni(i, red, green, blue);
    }
}

void MyPIXEL::closest(int azimuth, int red, int green, int blue, int num) {
    if (usePIXEL == true) {
        float ClosestPIXEL = (azimuth / 360 * NUMPIXEL);
        if ((ClosestPIXEL - (int)ClosestPIXEL) > 0.5) {
            ClosestPIXEL = (int)ClosestPIXEL + 1;
        } else {
            ClosestPIXEL = (int)ClosestPIXEL;
        }
        if (ClosestPIXEL >= 16) {
            ClosestPIXEL = 0;
        }
        if (num != 1) {
            int PIXELNUMstart = ClosestPIXEL - ((num - 1) / 2);
        } else {
            int PIXELNUMstart = ClosestPIXEL;
        }
        for (int i = 0; i < num; i++) {
            int j = PIXELNUMstart + i;
            if (j >= NUMPIXEL) {
                j -= 16;
            }
            mypixel.uni(j, red, green, blue);
        }
    }
}

void MyPIXEL::clear() {
    for (int i = 0; i < NUMPIXEL; i++) {
        PIXEL.setPixelColor(i, PIXEL.Color(0, 0, 0));
    }
    PIXEL.show();
}


void MyBUZZER::setup() {
    pinMode(BUZZER_PIN, OUTPUT);
}

void MyBUZZER::start(int BUZZERnote, int BUZZERduration) {
    if (BUZZERduration != 999) {
        int NoteDuration = 1000 / BUZZERduration;
        tone(BUZZER_PIN, BUZZERnote, NoteDuration);
        delay(BUZZERduration * 1.3);
        noTone(BUZZER_PIN);
        delay(10);
    } else {
        tone(BUZZER_PIN, BUZZERnote);
    }
}

void MyBUZZER::preset(int BUZZERpresetNUM) {
    switch (BUZZERpresetNUM) {
        case '1':
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
            break;       
    }
}