#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"

int getErr(int a, int b);
void resetUI();
void applyUI();

void Defense::setup() {
    // null
}

void Defense::defense_() {
    if (line.get_type() == 0) { // ラインなし 戻る
        mymotor.run(lastdetect, 180, 0);
        mybuzzer.start(1500, 999);
    } else { // ラインあり
        mybuzzer.stop();
        lastdetect = myvector.get_azimuth(line.get_x(), line.get_y());

        ball_move = true;
        if ((ball.get_azimuth() > (360 - ball_move_border) || ball.get_azimuth() < ball_move_border) && ball.get_stat() == 1) {
            ball_move = false;
        } else {
            Dtimer.reset();
        }

        if (ball_move) { // ボールを追いかける
            move_x = -ball_power;
            move_y = (line.get_y() * 5) * line_late;
            if (ball.get_azimuth() < 180) {
                move_x = ball_power;
            }
            if (line.get_type() == 2 || abs(line.get_x()) > 3) {
                if (diff_signs(line.get_x(), move_x)) {
                    move_x = line.get_x() * 10;
                    int lastmx = move_x;
                    if (((getErr(line.get_azimuth(), 90) < 5 || getErr(line.get_azimuth(), 270) < 5))) { // つまり縦ライン
                        if (!(ball.get_y() < 0)) {
                            move_y = 200;
                            move_x = lastmx / 2;
                        }
                    }
                }
            }
            move_azimuth = myvector.get_azimuth(move_x, move_y);
            move_power = myvector.get_magnitude(abs(move_x), move_y);
            if (ball.get_stat() == 1) {
                mymotor.run(move_azimuth, move_power, 0);
            } else {
                mymotor.free();
            }
        } else { // ボールがない → ラインのみ
            move_x = 0;
            move_y = (line.get_y() * 20) * line_late;
            move_azimuth = myvector.get_azimuth(move_x, move_y);
            move_power = myvector.get_magnitude(abs(move_x), move_y);
            if (move_power > 50) {
                mymotor.run(move_azimuth, move_power, 0);
            } else {
                mymotor.free();
            }
        }
    }
}

void resetUI() {
}

int getErr(int a, int b) {
    int diff = (a - b + 540) % 360 - 180;
    return diff;
}