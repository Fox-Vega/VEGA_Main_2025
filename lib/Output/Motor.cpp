#include "Motor.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

void MyMOTOR::setup() {
    for (int i = 0; i < 4; i++) {
        pinMode(motor_PIN1[i], OUTPUT);
        pinMode(motor_PIN2[i], OUTPUT);
    }
}

void MyMOTOR::run(int movement_azimuth, int power_, int dir_azimuth) {
    motor_azimuth = movement_azimuth;
    motor_magnitude = power_ * pwmscale;
    motor_stat = 1;
    dir_azimuth %= 360;
    int difix = 0;
    if (motor_stabilization && old_motor_stat == 1) {
        difix = mymotor.difix(dir_azimuth);
    }

    for (int i = 0; i < 4; i++) {
        if (motor_move == 1) {
            int azimuth_motor = (movement_azimuth - motor_degrees[i] + 360) % 360;

            // 座標計算
            myvector.get_cord(azimuth_motor, power_);
            int power = myvector.get_x();
            power += difix;
            power *= pwm_tweaker[i];
            power = constrain(power, -pwmlimit, pwmlimit);
            if (power >= 0) {
                analogWrite(motor_PIN1[i], 0);
                analogWrite(motor_PIN2[i], abs(power)); 
            } else {
                analogWrite(motor_PIN1[i], abs(power));
                analogWrite(motor_PIN2[i], 0); 
            }
            if (abs(power) < motor_border) {
                motor_stat = 0;
            }
        }
    }
    old_motor_stat = motor_stat;
}

int MyMOTOR::difix(int target_azimuth) {
    unsigned long dt = millis() - lastupdate;

    int current_azimuth = gam.get_azimuth();
    int error = (target_azimuth - current_azimuth + 540) % 360 - 180;

    // 微分項の計算（測定値の変化量ベース）
    float derivative = ((current_azimuth - prev_azimuth + 540) % 360 - 180) / dt;
    prev_azimuth = current_azimuth;

    // 微分先行型PD制御：積分項を削除
    int pwm = kd * derivative + kp * error;  // 順序：微分項が先行

    lastupdate = millis();

    return pwm;
}

void MyMOTOR::free() {
    for (int i = 0; i < 4; i++) {
        analogWrite(motor_PIN1[i], 0);
        analogWrite(motor_PIN2[i], 0);
    }
}

void MyMOTOR::brake() {
    for (int i = 0; i < 4; i++) {
        analogWrite(motor_PIN1[i], 255);
        analogWrite(motor_PIN2[i], 255);
    }
}

int MyMOTOR::get_azimuth() {
    return motor_azimuth;
}

int MyMOTOR::get_magnitude() {
    return motor_magnitude;
}

void MyMOTOR::stabilization(bool stat) {
    motor_stabilization = stat;
}

void MyMOTOR::move(bool stat) {
    motor_move = stat;
}
