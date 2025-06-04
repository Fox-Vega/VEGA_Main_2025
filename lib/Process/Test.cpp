#include "Test.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Test::test_() {
    if (myswitch.check_toggle() == 1) {
        for (int i = 0; i < 4; i++) {
            analogWrite(motor_PIN1[i], 255);
            analogWrite(motor_PIN2[i], 255);  
        }
        if (myswitch.check_tact() == 1) {
            t_mode -= 1;
            if (t_mode <= 0) {
                t_mode = 5;
            }
            delay(300);
        } else if (myswitch.check_tact() == 3) {
            t_mode += 1;
            if (t_mode >= 6) {
                t_mode = 1;
            }
            delay(300);
        }
        switch(t_mode) {
            case 1:
                mypixel.multi(0, 15, 255, 0, 0);
                mypixel.multi(4, 12, 255, 255, 255);
                mypixel.shows();
                break;
            case 2:
                mypixel.multi(0, 15, 255, 0, 0);
                mypixel.shows();
                break;
        }
        if ((millis() - lastbuzzer) > 1000) {
            mybuzzer.start(300, 30);
            lastbuzzer = millis();
        }
    } else {
        switch(t_mode) {
            case 1:
                test.input();
                break;
            case 2:
                test.motor();
                break;
            case 3:
                test.cord();
                break;
        }
    }
}


void Test::input() {
    ball.read();
    mypixel.multi(0, 15, 255, 255, 255);
    if (myswitch.check_tact() == 2){
        gam.dir_reset();
        mybuzzer.start(400, 50);
        delay(300);
    }
    int goal_azimuth = 0 - gam.get_azimuth();
    if (goal_azimuth < 0) {
        goal_azimuth += 360;
    }
    mypixel.closest(goal_azimuth, 255, 0, 100, 3);
    if (line.get_magnitude() != 999) {
        int line_azimuth = line.get_azimuth() + 180;
        line_azimuth %= 360;
        mypixel.closest(line_azimuth, 50, 255, 50, 3);
    }
    if (ball.get_magnitude() != 0) {
        int value = constrain(ball.get_magnitude(), 0, 255); //入力値を0~255の範囲に制限
        int r, g, b;
        r = 255;
        g = 255 - value; //緑をよりゆるやかに減らす
        b = 0;
        mypixel.closest(ball.get_azimuth(), r, g, b, 1);
    }

    mymotor.run(0, 0, 0);
    mypixel.shows();
}

void Test::motor() {
    while (myswitch.check_toggle() == 0) {
        for (int i = 0; i <= 140; i++) {
            for (int j = 0; j < 4; j++) {
                analogWrite(motor_PIN1[j], i);
                analogWrite(motor_PIN2[j], 0);
            }
        }
        for (int i = 140; i >= 0; i--) {
            for (int j = 0; j < 4; j++) {
                analogWrite(motor_PIN1[j], i);
                analogWrite(motor_PIN2[j], 0);
            }
        }
        for (int i = 0; i <= 140; i++) {
            for (int j = 0; j < 4; j++) {
                analogWrite(motor_PIN1[j], 0);
                analogWrite(motor_PIN2[j], i);
            }
        }
        for (int i = 140; i >= 0; i--) {
            for (int j = 0; j < 4; j++) {
                analogWrite(motor_PIN1[j], 0);
                analogWrite(motor_PIN2[j], i);
            }
        }
    }
    
    //全正転
    // analogWrite(motor_PIN1[0], 0);
    // analogWrite(motor_PIN2[0], 100);
    // analogWrite(motor_PIN1[1], 0);
    // analogWrite(motor_PIN2[1], 100);
    // analogWrite(motor_PIN1[2], 0);
    // analogWrite(motor_PIN2[2], 100);
    // analogWrite(motor_PIN1[3], 0);
    // analogWrite(motor_PIN2[3], 100);

    //全逆転
    // analogWrite(motor_PIN1[0], 100);
    // analogWrite(motor_PIN2[0], 0);
    // analogWrite(motor_PIN1[1], 100);
    // analogWrite(motor_PIN2[1], 0);
    // analogWrite(motor_PIN1[2], 100);
    // analogWrite(motor_PIN2[2], 0);
    // analogWrite(motor_PIN1[3], 100);
    // analogWrite(motor_PIN2[3], 0);
}

void Test::cord() {
    void setup();
    Serial.begin(9600);
}

void Test::cord() {
    if (ready == 0) {
        Serial.println("READY");  // Processingに開始信号を送信
        ready = 1;
    }
    Serial.print(5);  // 最初にデータの個数を送信
    Serial.print(",");

    Serial.print(gam.get_azimuth());  // 改行を追加して1行ずつ送信
    Serial.print(",");

    gam.get_cord();
    Serial.print(gam.get_x());
    Serial.print(",");
    Serial.print(gam.get_y());
    Serial.print(",");

    ball.read();
    myvector.get_cord((ball.get_azimuth() + gam.get_azimuth()) % 360, ball.get_magnitude());
    if (ball.get_magnitude() == 0) {
        Serial.print(9999);
        Serial.print(",");
        Serial.print(9999);
        Serial.println(",");
    } else {
        Serial.print(myvector.get_x() + gam.get_x());
        Serial.print(",");
        Serial.print(myvector.get_y() + gam.get_y());
        Serial.println(",");
    }
    delay(10);
}