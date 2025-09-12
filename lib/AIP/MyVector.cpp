#include "AIP.h"
#include "MyVector.h"

float MyVECTOR::get_azimuth(float x, float y) {
    theta = atan2(-y, x);
    azimuth = degrees(theta) - 90;
    if (azimuth < 0) {
        azimuth += 360;
    } else if (azimuth >= 360) {
        azimuth -= 360;
    }
    return azimuth;
}

float MyVECTOR::get_magnitude(float x, float y) {
    return sqrtf(x * x + y * y);
}

void MyVECTOR::get_cord(float azimuth, float magnitude) {
    if (magnitude < 0) { //反転
        azimuth += 180;
        if (azimuth >= 360) azimuth -= 360;
        magnitude = -magnitude;
    }
    theta = azimuth - 90;
    while (theta > 180) theta -= 360;
    while (theta < -180) theta += 360;
    x = round(cos(theta * M_PI / 180.0) * magnitude);
    y = -round(sin(theta * M_PI / 180.0) * magnitude);
}

float MyVECTOR::get_x() {
    return x;
}

float MyVECTOR::get_y() {
    return y;
}

float MyVECTOR::get_vectordegree(float ax, float ay, float bx, float by) {
    float a_magnitude = myvector.get_magnitude(ax, ay);
    float b_magnitude = myvector.get_magnitude(bx, by);
    if (a_magnitude == 0.0f || b_magnitude == 0.0f) return 0;
    double cos_theta = (ax * bx + ay * by) / (a_magnitude * b_magnitude);
    if (cos_theta > 1.0f) cos_theta = 1.0f;
    if (cos_theta < -1.0f) cos_theta = -1.0f;
    return degrees(acos(cos_theta));
}