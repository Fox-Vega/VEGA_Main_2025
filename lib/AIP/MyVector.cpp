#include "MyVector.h"
#include "Input.h"
#include "AIP.h"

void MyVECTOR::get_cord(int azimuth, int magnitude) {
    if (magnitude < 0) {
        azimuth += 180;
    }
    theta = 90 - azimuth; //方位角をy軸基準の角度に変換(+-180度)

    //値を調整
    if (theta < -180) {
        theta += 360;
    } else if (theta > 540) {
        theta -= 720;
    } else if (theta > 180) {
        theta -= 360;
    }

    //座標を計算
    x = static_cast<int>(round(cos(radians(theta)) * abs(magnitude)));
    y = static_cast<int>(round(sin(radians(theta)) * abs(magnitude)));
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
}

void MyVECTOR::get_tarcord(int tar_azimuth, int tar_magnitude) {
    int pl_x = gam.get_x();
    int pl_y = gam.get_y();

    myvector.get_cord(tar_azimuth, tar_magnitude);
    tarcord_x = (int)myvector.get_x() + pl_x;
    tarcord_y = (int)myvector.get_y() + pl_y;
}

void MyVECTOR::get_tarsvec(int old_tar_azimuth, int old_tar_magnitude, int tar_azimuth, int tar_magnitude, int old_cord_x, int old_cord_y, int lastupdatetime) {
    cord_x = gam.get_x();
    cord_y = gam.get_y();
    myvector.get_tarcord(old_tar_azimuth, old_tar_magnitude);
    old_tarcord_x = myvector.get_tar_x();
    old_tarcord_y = myvector.get_tar_y();
    myvector.get_tarcord(tar_azimuth, tar_magnitude);
    tarcord_x = myvector.get_tar_x();
    tarcord_y = myvector.get_tar_y();
    dt = millis() - lastupdatetime;

    //ズレを計算して秒間速度ベクトルに変換
    svec_x = (int)((cord_x - old_cord_x) / dt) * 1000; //自機の速度ベクトル
    svec_y = (int)((cord_y - old_cord_y) / dt) * 1000; //自機の速度ベクトル
    tarsvec_x = (int)(((tarcord_x - old_tarcord_x) / dt) * 1000) - svec_x; //ターゲットの速度ベクトル
    tarsvec_y = (int)(((tarcord_y - old_tarcord_y) / dt) * 1000) - svec_y; //ターゲットの速度ベクトル

    //ターゲットのベクトルから進行角(°)とマグニチュードを計算
    tarsvec_magnitude = myvector.get_magnitude(tarsvec_x, tarsvec_y);
    tarsvec_azimuth = myvector.get_azimuth(tarsvec_x, tarsvec_y);
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
    return (int)sqrt(x * x + y * y); //マグニチュードを計算、送信
}

//値取得ゾーン

int MyVECTOR::get_x() {
    return x;
}

int MyVECTOR::get_y() {
    return y;
}

int MyVECTOR::get_plpo_x() {
    return plpo_x;
}

int MyVECTOR::get_plpo_y() {
    return plpo_y;
}

int MyVECTOR::get_tar_x() {
    return tarcord_x;
}

int MyVECTOR::get_tar_y() {
    return tarcord_y;
}

int MyVECTOR::get_tarsvec_x() {
    return tarsvec_x;
}

int MyVECTOR::get_tarsvec_y() {
    return tarsvec_x;
}

int MyVECTOR::get_tarsvecazimuth() {
    return tarsvec_azimuth;
}

int MyVECTOR::get_tarsvecmagnitude() {
    return tarsvec_magnitude;
}
