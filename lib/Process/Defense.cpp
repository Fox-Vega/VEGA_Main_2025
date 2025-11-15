#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"

void Defense::setup(void) {

}

void Defense::reset() {
}

void Defense::defense_(void) {
    ball_azimuth = ball.get_azimuth();//メソッド呼び出しの削減
    mypixel.closest(ball_azimuth, 0, 0, 255, 1);
    ball_stat = ball.get_stat();
    line_azimuth = line.get_azimuth();
    line_stat = line.get_type();
    r_azimuth = gam.get_azimuth();
    calb = 0-r_azimuth;

    [this]() {
        if(line_stat == 0) {
            //ライン無し
            frog = FROG::NO_LINE;
            mybuzzer.start(500,999);
            return;
        } else if(ball_stat == false) {
            //ボール無し
            frog = FROG::NORMAL;
            return;
        } else {
            //ノーマル
            frog = FROG::NORMAL;
            return;
        }
    }();

    if(frog == FROG::NORMAL) {
        mybuzzer.stop();
        lastdetect = line_azimuth;
        if(isFront(line_azimuth)) {
            go_border[0] = line_azimuth;
            go_border[1] = norm360(line_azimuth + 180);
        } else {
            go_border[0] = norm360(line_azimuth + 180); //-かも
            go_border[1] = line_azimuth;
        }
        to_range(ball_azimuth, go_border[0], false);

        int go_flag;
        if(go_border[0] < ball_azimuth && ball_azimuth < go_border[1]) {
            go_flag = 0; // 範囲内
        } else {
            go_flag = 1; // 範囲外
        }

        int go_ang = go_border[go_flag] + 90; // 境界に垂直な角度
        norm360P(go_ang);

        if(line.get_type() == 2) {//縦ライン脱出
            if(getErr(0,line.get_pack(0))<100 && getErr(180,line.get_pack(1))<10) {//縦ライン
                if(getErr(ball_azimuth, line.get_pack(0)) < getErr(ball_azimuth, line.get_pack(1))) {
                    go_ang = line.get_pack(0);
                } else {
                    go_ang = line.get_pack(1);
                }
            } else {
                // if(getErr(180,go_ang)<20&&getErr(line.get_pack(0),line.get_pack(1))>100){//真後ろは避ける
                //     go_ang+=180;
                // }
            }
        }

        // go_angに向かって移動
        norm360P(go_ang);
        mymotor.run(go_ang+calb, 170, 0);
    } else if(frog == FROG::NO_LINE) {
        mymotor.run(lastdetect, return_power, 0);
    } else if(frog == FROG::NO_BALL) {
        mymotor.free();
    }
}

