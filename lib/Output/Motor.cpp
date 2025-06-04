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
    motor_stat = false;
    dir_azimuth %= 360;
    int difix = mymotor.difix(dir_azimuth);

    for (int i = 0; i < 4; i++) {
        int raw = movement_azimuth - motor_degrees[i];
        int azimuth_motor = ((raw % 360) + 360) % 360;
        
        // 座標計算
        myvector.get_cord(azimuth_motor, power_);
        float power = myvector.get_x();
        if (old_motor_stat == 1) {
            power += difix;
        }
        power = constrain(power, -pwmlimit, pwmlimit);
        if (power >= 0) {
            analogWrite(motor_PIN1[i], 0);
            analogWrite(motor_PIN2[i], abs(power)); 
        } else {
            analogWrite(motor_PIN1[i], abs(power));
            analogWrite(motor_PIN2[i], 0); 
        }
        if (power < motor_border) {
            motor_stat = false;
        }
    }
    old_motor_stat = motor_stat;
}

int MyMOTOR::difix(int target_azimuth) {
    float dt = (millis() - lastupdate) * 1.0f;
    dt = max(dt, 1.0f); // dtが極端に小さくならないように制限

    int current_azimuth = gam.get_azimuth();
    int error = (target_azimuth - current_azimuth + 540) % 360 - 180;

    // 微分項の計算（測定値の変化量ベース）
    float derivative = (current_azimuth - prev_azimuth) / dt;
    prev_azimuth = current_azimuth;

    // 微分先行型PD制御：積分項を削除
    int pwm = kd * derivative + kp * error;  // 順序：微分項が先行
    pwm *= pwmscale;

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
