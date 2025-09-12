#include"Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"


void Defense::setup(){
    //null
}

void Defense::defense_(){
    if(line.get_type() == 0){//
        Serial.println("no line");
    } else {//ラインあり
        ball_move=true;
        if(ball.get_azimuth()>(360-ball_move_border)||ball.get_azimuth()<ball_move_border){
            ball_move=false;
        } else {
            Dtimer.reset();
        }
        Serial.println("ball:"+String(move_x)+"  line_y:"+String(line.get_y()));

        if(ball_move){//ボールを追いかける
            move_x=ball_power;
            if(ball.get_azimuth()>180){
                move_x=-ball_power;
            }
            move_y=line.get_y
        } else {//ラインのみ

        }
    }
}