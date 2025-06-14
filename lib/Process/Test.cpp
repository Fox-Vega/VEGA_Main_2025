#include "Test.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Test::test_() {
    mypixel.multi(0, 15, 255, 255, 255);
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
                mypixel.uni(0, 255, 0, 0);
                mypixel.multi(7, 9, 255, 0, 0);
                break;
            case 2:
                mypixel.multi(0, 15, 255, 0, 0);
                break;
            case 3:
                mypixel.uni(0, 0, 0, 255);
                mypixel.uni(4, 255, 0, 0);
                mypixel.uni(8, 0, 0, 255);
                mypixel.uni(12, 255, 0, 0);
                break;
            default:
                break;
            gam.dir_reset();
        }
        if ((millis() - lastbuzzer) > 1000) {
            mybuzzer.start(300, 30);
            lastbuzzer = millis();
        }
        motor_mode = 2;
        motor_speed = 0;
        mymotor.stabilization(1);
        mymotor.move(1);
    } else {
        mypixel.clear();
        switch(t_mode) {
            case 1:
                test.input();
                break;
            case 2:
                test.motor();
                break;
            case 3:
                test.processing();
                break;
        }
    }
    mypixel.shows();
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
    // if (line.get_magnitude() != 999) {
    //     int line_azimuth = line.get_azimuth() + 180;
    //     line_azimuth %= 360;
    //     mypixel.closest(line_azimuth, 50, 255, 50, 3);
    // }
    if (ball.get_value(99) != 0) {
        int value = constrain(ball.get_value(99) / 4, 0, 255); //入力値を0~255の範囲に制限
        int r, g, b;
        r = 255;
        g = 255 - value; //緑をよりゆるやかに減らす
        b = 0;
        mypixel.closest(ball.get_azimuth(), r, g, b, 1);
    }
}

void Test::motor() {
    mymotor.stabilization(0);
    if (myswitch.check_tact() == 3) {
        if (motor_mode != 1) {
            motor_speed = 0;
        } else {
            motor_speed += 10;
        }
        motor_mode = 1;
        old_motor_mode = motor_mode;
        old_motor_speed = motor_speed;
        delay(200);
    } else if (myswitch.check_tact() == 2) {
        if (motor_speed != 0) {
            motor_mode = 2;
            motor_speed = 0;
        } else {
            motor_mode = old_motor_mode;
            motor_speed = old_motor_speed;
        }
        delay(200);
    } else if (myswitch.check_tact() == 1) {
        if (motor_mode != 3) {
            motor_speed = 0;
        } else {
            motor_speed += 10;
        }
        old_motor_mode = motor_mode;
        old_motor_speed = motor_speed;
        motor_mode = 3;
        delay(200);
    }
    if (motor_mode == 1) {
        mymotor.run(0, motor_speed, 0);
    } else if (motor_mode == 3) {
        mymotor.run(180, motor_speed, 0);
    } else {
        mymotor.brake();
    }
    if (motor_mode != 2) {
        if (motor_mode == 1) {
            if (motor_speed == 0) {
                mypixel.uni(0, 255, 100, 100);
            } else {
                mypixel.multi(0, motor_speed / 10, 255, 0, 0);
            }
        } else {
            if (motor_speed == 0) {
                mypixel.uni(0, 100, 100, 255);
            } else {
                byte startPIXEL = (16 - (motor_speed / 10)) % 16;
                mypixel.uni(0, 0, 0, 255);
                mypixel.multi(startPIXEL, 15, 0, 0, 255);
            }
        }
    } else {
        mypixel.uni(0, 255, 0, 255);
        mypixel.uni(2, 255, 0, 255);
        mypixel.uni(4, 255, 0, 255);
        mypixel.uni(6, 255, 0, 255);
        mypixel.uni(8, 255, 0, 255);
        mypixel.uni(10, 255, 0, 255);
        mypixel.uni(12, 255, 0, 255);
        mypixel.uni(14, 255, 0, 255);
    }
}

void Test::processing() {
    mymotor.move(0);
    if (myswitch.check_tact() == 2) {
        serial_mode += 1;
        serial_mode %= 2; //モード個数
        delay(200);
    }

    Serial.print(8);  // 最初にデータの個数を送信
    Serial.print(",");

    Serial.print(serial_mode);
    Serial.print(",");
    Serial.print((gam.get_azimuth() + 180) % 360);  // 改行を追加して1行ずつ送信
    Serial.print(",");

    // gam.get_cord();
    // Serial.print(582 - gam.get_x());
    // Serial.print(",");
    // Serial.print(430 - gam.get_y());
    // Serial.print(",");
    Serial.print(582);
    Serial.print(",");
    Serial.print(430);
    Serial.print(",");

    ball.read();
    int ball_azimuth = (ball.get_azimuth() + gam.get_azimuth() + 90) % 360;
    myvector.get_cord(ball_azimuth , ball.get_magnitude());
    if (ball.get_magnitude() == 0) {
        Serial.print(0);
        Serial.print(",");
        Serial.print(0);
        Serial.print(",");
    } else {
        Serial.print(myvector.get_x() * 0.5);
        Serial.print(",");
        Serial.print(-myvector.get_y() * 0.5);
        Serial.print(",");
    }
    attack.attack_();
    Serial.print(mymotor.get_azimuth());
    Serial.print(",");
    Serial.println(mymotor.get_magnitude());
    // Serial.print(",");

    delay(10);
}
