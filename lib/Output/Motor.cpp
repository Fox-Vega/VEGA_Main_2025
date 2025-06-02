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
    // 方向制御の処理（今回は使っていません）
    dir_azimuth %= 360;
    int difix = mymotor.difix(dir_azimuth);

    for (int i = 0; i < 4; i++) {
        int raw = movement_azimuth - motor_degrees[i];
        int azimuth_motor = ((raw % 360) + 360) % 360;
        
        // 座標計算
        myvector.get_cord(azimuth_motor, power_);
        float power = myvector.get_x();
        power = power + difix;
        power = constrain(power, -pwmlimit, pwmlimit);
        if (power > 0) {
            analogWrite(motor_PIN1[i], 0);
            analogWrite(motor_PIN2[i], abs(power)); 
        } else if (power < 0){
            analogWrite(motor_PIN1[i], abs(power));
            analogWrite(motor_PIN2[i], 0); 
        } else {
            mymotor.brake();
        }
    }
}

int MyMOTOR::difix(int target_azimuth) {
    float dt = millis() - lastupdate;
    int current_azimuth = gam.get_azimuth();
    int error = (target_azimuth - current_azimuth + 360) % 360;
    if (error > 180) error -= 360;  // 短い角度方向で調整

    // 積分の制限を追加
    integral += error * dt;
    integral = constrain(integral, -integrallimit, integrallimit);

    float derivative = (error - prev_error) / dt;
    prev_error = error;

    int pwm = kp * error + ki * integral + kd * derivative;
    pwm *= pwmscale;
    pwm = constrain(pwm, -difixlimit, difixlimit);  // PWMの範囲制限
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
