#include "Motor.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

 //TODO もし姿勢制御が反対向きになっていた場合、motor.move の中の　+　-　を変更すること。
 //TODO もしモーターが逆回転していた場合、PIN1とPIN2の内容を反転させること。

void MyMOTOR::setup() {
    for (int i = 0; i < 4; i++) {
        pinMode(motor_PIN1[i], OUTPUT);
        pinMode(motor_PIN2[i], OUTPUT);
    }
}

void MyMOTOR::run(int movement_azimuth, int power_, int dir_azimuth) {
    if (movement_azimuth >= 360) {
        movement_azimuth -= 360;
    }
    if (dir_azimuth >= 360) {
        dir_azimuth -= 360;
    }
    difixPWM = mymotor.difix(dir_azimuth); //姿勢制御の値
    for (int i = 0; i < 4; i++) {
        azimuth_motor = movement_azimuth - motor_degrees[i]; //オムニの軸がy軸になるようにする
        myvector.get_cord(azimuth_motor, power_ - abs(difixPWM)); //座標計算
        power = myvector.get_x(); //x座標を取得(モーターの回転速度)
        if (power >= 0) {   
            PoMi = true;
        } else {
            PoMi = false;
            power -= (power * 2);
        }
        if (PoMi == true) {
            analogWrite(motor_PIN1[i], (int)(power - difixPWM));
            analogWrite(motor_PIN2[i], 0);
        } else {
            analogWrite(motor_PIN1[i], 0);
            analogWrite(motor_PIN2[i], (int)(power + difixPWM));
        }
    }
}

int MyMOTOR::difix(int setpoint) {
    double error = setpoint - gyro.get_azimuth();
    integral += error;
    derivative = error - prev_error;
    prev_error = error;
    double angularVelocity = kp * error + ki * integral + kd * derivative;
    if (angularVelocity >= 0) {
        difix_PoMi = true;
    } else {
        difix_PoMi = false;
    }
    return motorPWM = (int)constrain(abs(angularVelocity) * pwmscale, 0, 255);
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