#include "AIP.h"


int MyVECTOR::get_azimuth(int x, int y) {
    theta = atan2(y, x); //座標をy軸基準の角度に変換(+-180度)
    azimuth = (int)90 - degrees(theta); //y軸基準の角度を方位角に変換

    //値を調整
    if (azimuth < 0) {
        azimuth += 360;
    } else if (azimuth > 360) {
        azimuth -= 360;
    }

    return azimuth;
}

float MyVECTOR::get_magnitude(int x, int y) {
    return (float)sqrt((double)x * x + (double)y * y); //マグニチュードを計算、送信
}

void MyVECTOR::get_cord(int azimuth, int magnitude) {
    if (magnitude < 0) { //反転
        azimuth = (azimuth + 180) % 360;
        magnitude = -magnitude;
    }
    
    theta = 90 - azimuth; //方位角をy軸基準の角度に変換
    
    //thetaの範囲を（-180, 180に）正規化
    while (theta > 180) theta -= 360;
    while (theta < -180) theta += 360;

    //座標を計算
    x = (int)round(cos(radians(theta)) * magnitude);
    y = (int)round(sin(radians(theta)) * magnitude);
}

int MyVECTOR::get_x() {
    return x;
}

int MyVECTOR::get_y() {
    return y;
}

int get_vectordegree(int ax, int ay, int bx, int by) {
    float dot = (ax * bx) + (ay * by);
    float a_magnitude = myvector.get_magnitude(ax, ay);
    float b_magnitude = myvector.get_magnitude(bx, by);
    double cos_theta = dot / (a_magnitude * b_magnitude);
    if (cos_theta > 1.0) cos_theta = 1.0;
    if (cos_theta < -1.0) cos_theta = -1.0;
    int vectordegree = degrees(acos(cos_theta));
    return vectordegree;
}