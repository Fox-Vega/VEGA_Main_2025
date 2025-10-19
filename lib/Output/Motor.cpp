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
    max_power = constrain(power_, -pwmlimit, pwmlimit); //制限かける

    motor_azimuth = movement_azimuth; //記録用
    motor_magnitude = max_power * pwmscale;

    int difix = 0;
    if (motor_stabilization) {
        difix = mymotor.difix(dir_azimuth);
    }

    h = 0; //最高値
    for (int i = 0; i < 4; i++) {
        int azimuth_motor = (movement_azimuth - motor_degrees[i] + 360) % 360; //モーター軸を0度とした時の進行方向

        // 座標計算
        myvector.get_cord(azimuth_motor, max_power); //姿勢制御分の出力を引かずに計算
        motor_power_[i] = myvector.get_x(); //仮出力

        if (abs(motor_power_[i]) > h) {
            h = abs(motor_power_[i]);
        }
    }

    for (int i = 0; i < 4; i++) {
        pp = abs(motor_power_[i]) / abs(h);

        if (motor_power_[i] > 0) power = ((max_power - abs(difix)) * pp) + difix;
        else if (motor_power_[i] < 0) power = ((-max_power + abs(difix)) * pp) + difix;
        else power = difix;

        power = constrain(power, -pwmlimit, pwmlimit);
        if (motor_move == 1) {
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

void MyMOTOR::run_non_stabilization(int movement_azimuth, int power_) {
    max_power = constrain(power_, -pwmlimit, pwmlimit); //制限かける

    motor_azimuth = movement_azimuth; //記録用
    motor_magnitude = max_power * pwmscale;

    h = 0; //最高値
    for (int i = 0; i < 4; i++) {
        int azimuth_motor = (movement_azimuth - motor_degrees[i] + 360) % 360; //モーター軸を0度とした時の進行方向

        // 座標計算
        myvector.get_cord(azimuth_motor, max_power); //姿勢制御分の出力を引かずに計算
        motor_power_[i] = myvector.get_x(); //仮出力

        if (abs(motor_power_[i]) > h) {
            h = abs(motor_power_[i]);
        }
    }

    for (int i = 0; i < 4; i++) {
        pp = abs(motor_power_[i]) / abs(h);

        if (motor_power_[i] > 0) power = (max_power * pp);
        else if (motor_power_[i] < 0) power = (-max_power * pp);
        else power = 0;

        power = constrain(power, -pwmlimit, pwmlimit);
        if (motor_move == 1) {
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
    float dt = (millis() - lastupdate) / 1000.0; //秒に直す
    int current_azimuth = gam.get_azimuth();

    //角度差を求める
    float error = target_azimuth - current_azimuth;
    if (error > 180) { //-180 ~ 180　に収める
        error -= 360;
    } else if (error < -180) {
        error += 360;
    }

    //微分項の計算
    int dif = current_azimuth - prev_azimuth;
    if (dif > 180) { //-180 ~ 180　に収める
        dif -= 360;
    } else if (dif < -180) {
        dif += 360;
    }
    float derivative = dif / dt;

    // 微分先行型PD
    int pwm = kp * error - kd * derivative;

    pwm = constrain(pwm, -pwmlimit, pwmlimit);

    lastupdate = millis();
    prev_azimuth = current_azimuth;
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