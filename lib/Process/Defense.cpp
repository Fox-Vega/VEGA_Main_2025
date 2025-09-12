#include"Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"


void Defense::setup(){
    //null
}

void Defense::defense_(){
    if(line.get_type() == 0){//
        mymotor.run(lastdetect, 180, 0);
        mybuzzer.start(1500,999);
        mypixel.closest(lastdetect,255,0,0,1);
    } else {//ラインあり
        mybuzzer.stop();
        lastdetect=myvector.get_azimuth(line.get_x(),line.get_y());
        mypixel.closest(lastdetect,0,255,0,1);
        Serial.println("line y:"+String(line.get_y())+" x:"+String(line.get_x())+" azimuth:"+String(line.get_azimuth()));

        ball_move=true;
        if((ball.get_azimuth()>(360-ball_move_border)||ball.get_azimuth()<ball_move_border)&&ball.get_stat()==1){
            ball_move=false;
        } else {
            Dtimer.reset();
        }

        if(ball_move){//ボールを追いかける
            move_x=ball_power;
            if(ball.get_azimuth()<180){
                move_x=-ball_power;
            }
            move_y=(line.get_y()/10)*line_late;
            move_azimuth=myvector.get_azimuth(move_x,move_y);
            move_power=myvector.get_magnitude(move_x,move_y);
            if(ball.get_stat()==1){
                mymotor.run(move_azimuth,move_power,0);
            } else {
                mymotor.free();
            }
        } else {//ラインのみ
            mymotor.free();
        }
    }
}