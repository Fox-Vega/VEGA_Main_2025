#pragma once
#include <Arduino.h>
#include <Timer.h>

extern const int back_ang[4];
extern Timer SilentTime;
extern Timer MoveTime;
extern int LastReturn;



class Defense{
public:
    void setup(void);
    void defense_(int start_cord);
    void reset(void);
    int dhstget(void);
private:
    void dash(void);

    inline void norm360P(int &a) {
        a %= 360;
        if(a < 0) a += 360;
    }

    inline int norm360(int a) {
        a %= 360;
        if(a < 0) a += 360;
        return a;
    }

    inline bool isFront(int angle) {
        norm360P(angle);
        return (angle <=90||angle>=270);
    }

    inline bool diff_signs(int a, int b) {
        return (a > 0 && b < 0) || (a < 0 && b > 0);
    }

    inline bool isInSide30(int angle) {
        angle = norm360(angle);
        return (angle >= 60 && angle <= 120) || (angle >= 240 && angle <= 300);
    }

    inline static int getErr( int a, int b) {
        int d = abs((a - b) % 360);
        return (d > 180) ? (360 - d) : d;
    }

    inline static float scaleRange(float min1, float max1, float min2, float max2, float value) {
        if(value <= min1) return min2;
        if(value >= max1) return max2;
        return (value - min1) / (max1 - min1) * (max2 - min2) + min2;
    }

};
