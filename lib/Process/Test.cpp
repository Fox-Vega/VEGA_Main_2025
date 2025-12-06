#include "Test.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

bool isPixelActive = false; // pixel() 実行中フラグ

void Test::test_(){
    exit = false;
    t_mode = 1; // 初期モードを設定
    lastPixelState = mypixel.stat();
    //if(!lastPixelState)mypixel.use_pixel(1);

    while (!exit) {
        gam.read_azimuth();
        mypixel.use_pixel(1);
        mypixel.multi(0, 15, 255, 255, 255); //ベースの白

        if (myswitch.check_toggle() == 1) { //off
            gam.dir_reset();
            motor_mode = 2;
            motor_speed = 0;
            mymotor.stabilization(1);
            mymotor.move(1);
            mymotor.free();

            //モード変更
            if (myswitch.check_tact() == 1) {
                t_mode -= 1;
                if (t_mode <= 0) t_mode = modeNUM;
                delay(200);
            } else if (myswitch.check_tact() == 9) {
                t_mode += 1;
                if (t_mode > modeNUM) t_mode = 1;
                delay(200);
            }

            switch(t_mode) {
                case 1: //Input
                    mypixel.uni(0, 255, 0, 0);
                    mypixel.multi(7, 9, 255, 0,0);
                    break;
                case 2:
                    mypixel.closest(0, 100, 255, 100, 3);
                    break;
                case 3: //Motor
                    mypixel.multi(0, 15, 255, 0, 1);
                    break;
                case 4: //Stabilize
                    mypixel.multi(0, 1, 255, 255, 0);
                    mypixel.multi(7, 15, 255, 255, 0);
                    break;
                case 5:
                    mypixel.multi(0, 15, 57, 197, 187);
                    break;
                case 6: //Exit
                    mypixel.rainbow();
                    break;
                default:
                    break;
            }

            //待機音
            if (timer.read_milli() > 1000) {
                mybuzzer.start(300, 30);
                timer.reset();
            }
            crr=0;
        } else {
            mypixel.clear();
            switch(t_mode) {
                case 1:
                    test.input();
                    break;
                case 2:
                    test.escape();
                    break;
                case 3:
                    test.motor();
                    break;
                case 4:
                    test.stabilize();
                    break;
                case 5:
                    test.free();
                    break;
                case 6:
                    mybuzzer.start(100, 500);
                    if(!lastPixelState)mypixel.use_pixel(0);
                    exit = true; // Exit the test loop
                    break;
            }
        }
        mypixel.shows();
    }
}


void Test::input() {
    if (myswitch.check_tact() == 1){
        mybuzzer.start(200, 999);
    } else if (myswitch.check_tact() == 5) {
        mybuzzer.start(300, 999);
    } else if (myswitch.check_tact() == 9) {
        mybuzzer.start(400, 999);
    } else {
        mybuzzer.stop();
    }

    mypixel.multi(0, 15, 255, 255, 255); //ベースの白

    //正面
    int zero_azimuth = 0 - gam.get_azimuth();
    if (zero_azimuth < 0) zero_azimuth += 360;
    mypixel.closest(zero_azimuth, 255, 0, 100, 3);

    //ボール
    ball.read();
    if (ball.get_stat() != 0) mypixel.closest(ball.get_azimuth(), 255, 255, 0, 1);
    int ball_azimuth = ball.get_azimuth();
    if (ball_azimuth > 180) ball_azimuth -= 360; //-179 ~ 180
    Serial.println(ball_azimuth);
}

void Test::escape() {
    line.read();
    if (line.get_type() != 0) {
        mymotor.stabilization(0); //姿勢制御無効化
        mypixel.closest(line.get_azimuth(), 100, 100, 255, 1);
        mypixel.closest(line.get_eazimuth(), 100, 255, 100, 3);
        mymotor.run(line.get_eazimuth(), 200, 0);
    } else {
        mymotor.free();
    }
}

void Test::motor() {
    mymotor.stabilization(0); //姿勢制御無効化
    if (myswitch.check_tact() == 9) {
        if (motor_mode != 1) motor_speed = 0;
        else motor_speed += 10;

        motor_mode = 1;
        old_motor_mode = motor_mode;
        delay(200);
    } else if (myswitch.check_tact() == 5) {
        if (motor_speed != 0) motor_mode = 2;
        else motor_mode = old_motor_mode;
        delay(200);
    } else if (myswitch.check_tact() == 1) {
        if (motor_mode != 3) motor_speed = 0;
        else motor_speed += 10;

        motor_mode = 3;
        old_motor_mode = motor_mode;
        delay(200);
    }

    if (motor_mode == 1) {
        mymotor.run(0, motor_speed, 0);
        if (motor_speed == 0) {
            mypixel.uni(0, 255, 100, 100);
        } else {
            mypixel.multi(0, motor_speed / 10, 255, 0, 0);
        }
    } else if (motor_mode == 3) {
        mymotor.run(180, motor_speed, 0);
        if (motor_speed == 0) {
            mypixel.uni(0, 100, 100, 255);
        } else {
            mypixel.uni(0, 0, 0, 255);
            mypixel.multi((16 - (motor_speed / 10)) % 16, 15, 0, 0, 255);
        }
    } else {
        mymotor.free();
        mypixel.uni(0, 255, 0, 255);
        mypixel.uni(2, 255, 0, 255);
        mypixel.uni(4, 255, 0, 255);
        mypixel.uni(6, 255, 0, 255);
        mypixel.uni(8, 255, 0, 255);
        mypixel.uni(10, 255, 0, 255);
        mypixel.uni(12, 255, 0, 255);
        mypixel.uni(14, 255, 0, 255);
    }
}

void Test::stabilize() {
    mymotor.run(0, 0, 0);
}

// void Test::free() {//グルグルするやつ　左に進むだけ
//     // //なんでもいれていいところ　内容書いてね->　アタックの進行方向

//     // mymotor.move(0);
//     // ball.read();
//     // attack.attack_();

//     // Serial.print(ball.get_azimuth());
//     // Serial.print(" ");
//     // Serial.println(mymotor.get_azimuth());
//     crr++;
//     mypixel.multi(0, 15, 255, 255, 255);
//     mypixel.closest(270-gam.get_azimuth(), 255, 0, 0, 1);
//     mypixel.closest(crr%360, 255, 255, 0, 1);
//     mymotor.run(270-gam.get_azimuth(), 140,(crr%720)/2);
// }

inline static int getErr( int a, int b) { int d = abs((a - b) % 360); return (d > 180) ? (360 - d) : d; }

void Test::free(){
    line.read();
    bool tl = false;
    bool frog1 = line.get_stat(0) || line.get_stat(1) || line.get_stat(2) ||
                line.get_stat(23) || line.get_stat(22);
    bool frog2 = line.get_stat(11) || line.get_stat(12) || line.get_stat(13) ||
                line.get_stat(10) || line.get_stat(9);
    if(frog1 && frog2) {
        tl = true;
    }
    if(tl){
        mybuzzer.start(400, 999);
    }else{
        mybuzzer.stop();
    }
}