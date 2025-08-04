#include "MyVector.h"
#include "Input.h"
#include "AIP.h"

void MyVECTOR::get_cord(int azimuth, int magnitude) {
    if (magnitude < 0) {
        azimuth = (azimuth + 180) % 360;
        magnitude = -magnitude;
    }
    
    theta = 90 - azimuth; // 方位角をy軸基準の角度に変換
    
    // thetaの範囲を[-180, 180]に正規化
    theta = fmod(theta + 180 + 360, 360) - 180;

    // 座標を計算
    double radian_theta = theta * M_PI / 180.0;
    x = (int)round(cos(radian_theta) * magnitude);
    y = (int)round(sin(radian_theta) * magnitude);
}


void MyVECTOR::get_plpocord(int po_x, int po_y) {
    int pl_x = gam.get_x();
    int pl_y = gam.get_y();

    //差の絶対値を取得
    plpo_x = abs(po_x - pl_x);
    plpo_y = abs(po_y - pl_y);

    //距離の正・負を適用
    if (po_x < pl_x) {
        plpo_x = plpo_x - (plpo_x * 2);
    } else if (po_y < plpo_y) {
        plpo_y = plpo_y - (plpo_y * 2);
    }
    x = plpo_x;
    y = plpo_y;
}

void MyVECTOR::get_tarcord(int tar_azimuth, int tar_magnitude) {
    int pl_x = gam.get_x();
    int pl_y = gam.get_y();

    myvector.get_cord(tar_azimuth, tar_magnitude);
    tarcord_x = (int)myvector.get_x() + pl_x;
    tarcord_y = (int)myvector.get_y() + pl_y;
    x = tarcord_x;
    y = tarcord_y;
}

void MyVECTOR::get_svec(int tar_azimuth, int tar_magnitude) {
    cord_x = gam.get_x();
    cord_y = gam.get_y();
    myvector.get_tarcord(tar_azimuth, tar_magnitude);
    tarcord_x = myvector.get_x();
    tarcord_y = myvector.get_y();
    dt = millis() - lastupdatetime;

    //ズレを計算して秒間速度ベクトルに変換
    svec_x = (int)((cord_x - lastcord_x) / dt) * 1000; //自機の速度ベクトル
    svec_y = (int)((cord_y - lastcord_y) / dt) * 1000; //自機の速度ベクトル
    tarsvec_x = (int)(((tarcord_x - lasttar_x) / dt) * 1000) - svec_x; //ターゲットの速度ベクトル
    tarsvec_y = (int)(((tarcord_y - lasttar_y) / dt) * 1000) - svec_y; //ターゲットの速度ベクトル

    //値の更新
    lasttar_x = tarcord_x;
    lasttar_y = tarcord_y;
    lastcord_x = cord_x;
    lastcord_y = cord_y;
    x = tarsvec_x;
    y = tarsvec_y;
}

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

int MyVECTOR::get_magnitude(int x, int y) {

    return (int)sqrt((double)x * x + (double)y * y); // マグニチュードを計算、送信
}

double MyVECTOR::dot_product(double origin_x, double origin_y, double point1_x, double point1_y, double point2_x, double point2_y) {
    // ベクトル構築
    double v1_x = point1_x - origin_x;
    double v1_y = point1_y - origin_y;
    
    double v2_x = point2_x - origin_x;
    double v2_y = point2_y - origin_y;

    // 内積とノルム
    double dot = v1_x * v2_x + v1_y * v2_y;
    double mag1 = sqrt(v1_x * v1_x + v1_y * v1_y);
    double mag2 = sqrt(v2_x * v2_x + v2_y * v2_y);

    // 角度計算
    double cos_theta = dot / (mag1 * mag2);
    if (cos_theta > 1.0) cos_theta = 1.0;
    if (cos_theta < -1.0) cos_theta = -1.0;

    double angle_rad = acos(cos_theta);
    double angle_deg = angle_rad * rad_to_deg;

    return angle_deg;
}


//値取得ゾーン

int MyVECTOR::get_x() {
    return x;
}

int MyVECTOR::get_y() {
    return y;
}