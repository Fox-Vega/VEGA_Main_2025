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
    max_power = constrain(power_, -pwmlimit, pwmlimit);

    motor_azimuth = movement_azimuth;
    motor_magnitude = max_power * pwmscale;

    int difix = 0;
    if (motor_stabilization) {
        difix = mymotor.difix(dir_azimuth);
    }

    for (int i = 0; i < 4; i++) {
        int azimuth_motor = (movement_azimuth - motor_degrees[i] + 360) % 360;

        // 座標計算
        myvector.get_cord(azimuth_motor, max_power - abs(difix));
        motor_power_[i] = myvector.get_x();
        if (motor_power_[i] > h) {
            h = motor_power_[i];
        }
    }
    for (int i = 0; i < 4; i++) {
        pp = motor_power_[i] / h;
        power = ((max_power - abs(difix)) * pp) + difix;

        if (motor_move == 1) {
            power = constrain(power, -pwmlimit, pwmlimit);

            if (power >= 0) {
                analogWrite(motor_PIN1[i], 0);
                analogWrite(motor_PIN2[i], abs(power)); 
            } else {
                analogWrite(motor_PIN1[i], abs(power));
                analogWrite(motor_PIN2[i], 0); 
            }
        }
    }
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

    pwm = constrain(pwm, -pwmlimit, pwmlimit);

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