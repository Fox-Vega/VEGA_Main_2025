#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
//#include "okomeonigiri.h"

void Defense::setup(void){
    mybuzzer.start(1000, 200);
    general.setup();
}

void Defense::defense_(void){
    setup();
    while(true){
        get_vector();
        if(line_detect){
            if(line.get_type()==1){
                p1();
            }
        }
        else{
            while(1){
                GoBackLine();
                get_vector();
                if(line_detect)break;
            }
        };
    }
}

void Defense::p1(void){
    int move_x=0;
    int move_y=0;
    move_y=line_y
}

void Defense::GoBackLine(){
    int last_line_time=0;
    for(byte i=0; i<50;i++){
        if(line_history[0][50-i]==1){
            last_line_time=50-1;
            break;
        }
    }
    mymotor.run(line_history[1][last_line_time],120,0);
}

void Defense::get_vector(void)
{
    // ライン情報の取得
    line.read();
    line_azimuth = line.get_azimuth();
    line_dist = line.get_magnitude() * ball_max / 22;
    line_detect = (line_dist == 999) ? false : true;
    line_x = line_dist * cos(radians(line_azimuth));
    line_y = line_dist * sin(radians(line_azimuth));

    // 履歴保存（history 0:detect 1:azimuth 2:dist）
    if (line_history_index >= 50) {
        for (int i = 1; i < 50; i++) {
            line_history[0][i - 1] = line_history[0][i];
            line_history[1][i - 1] = line_history[1][i];
            line_history[2][i - 1] = line_history[2][i];
        }
        line_history_index = 49;
    }
    line_history[0][line_history_index] = line_detect;
    line_history[1][line_history_index] = line_azimuth;
    line_history[2][line_history_index] = line_dist;
    line_history_index++;

    // ボール情報の取得
    ball.read();
    ball_azimuth = ball.get_azimuth();
    ball_dist = (int)ball.get_magnitude();
    ball_detect = ((int)ball.get_magnitude() > 0) ? true : false;
    myvector.get_cord(ball_azimuth, ball_dist);
    ball_x = myvector.get_x();
    ball_y = myvector.get_y();

    // 履歴保存（history 0:detect 1:azimuth 2:dist）
    if (ball_history_index >= 50) {
        for (int i = 1; i < 50; i++) {
            ball_history[0][i - 1] = ball_history[0][i];
            ball_history[1][i - 1] = ball_history[1][i];
            ball_history[2][i - 1] = ball_history[2][i];
        }
        ball_history_index = 49;
    }
    ball_history[0][ball_history_index] = ball_detect;
    ball_history[1][ball_history_index] = ball_azimuth;
    ball_history[2][ball_history_index] = ball_dist;
    ball_history_index++;
}
