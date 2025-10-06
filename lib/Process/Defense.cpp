#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"


// ===================================
// セットアップ処理
// ===================================

void Defense::setup() {
    line_r = line.get_R();
    reset();
}

// ===================================
// メイン処理
// ===================================

void Defense::defense_() {
    bool tl=false;
    if (!timer_started) {
        timer_started = true;
    }
    frog = 0;

    {
        bool frog1=false;
        bool frog2=false;
        int leng=4;
        for(int i=0; i<leng;i++){
            if(line.get_stat(i)==true){
                frog1=true;
                break;
            }
        }
        for(int i=1; i<leng;i++){
            if(line.get_stat(24-i)==true){
                frog1=true;
                break;
            }
        }
        for(int i=0; i<leng;i++){
            if(line.get_stat(11+i)==true){
                frog2=true;
                break;
            }
        }
        for(int i=1; i<leng;i++){
            if(line.get_stat(12-i)==true){
                frog2=true;
                break;
            }
        }
        if(frog1&&frog2){
            tl=true;
        }
    }

    // === ダッシュ処理 ===
    [this](){if(SilentTime.read_milli()>dash_border&& USE_DASH == true){ //なんでラムダで囲んでるかって？ まあ気分だよ気分
        float TL = 20.0;        //TL＝縦　ライン　(脳筋)
        float TLM = 20.0;        //TL＝縦　ライン　(脳筋)
        if(SilentTime.read_milli()<dash_border*1.5||1){//最初に起動防止　まあほぼ意味はないけど
            mypixel.multi(0,15,255,50,50);mypixel.show();
            SilentTime.reset();

            //少し前に進みます
            while(SilentTime.read_milli()<300){gam.read_azimuth();mymotor.run(0,200,0);}SilentTime.reset();


            while(SilentTime.read_milli()<dash_time){//こちらがメイン　アタxック呼び出してるだけ　頭悪い
                //while文の弊害たち
                gam.read_azimuth();
                ball.read();
                line.read();
                if(line.get_type()!=0){lastdetect=line.get_azimuth();}
                attack.attack_();
            }SilentTime.reset();

            //少し後ろに進みます
            while(SilentTime.read_milli()<300){gam.read_azimuth();mymotor.run(180,200,0);}SilentTime.reset();

            int mm=180; //ムーブ向き　　ネーミングセンスは-100点
            int mt=100; //ムーブ力　　　名前の付け方頭悪い

            while(1){ //戻ろう

                if(line.get_type()==1){//縦ライン見えたら少しずらす　水平やったら抜けますね
                    if((line.get_azimuth()<180-TL&&line.get_azimuth()>TL)||(line.get_azimuth()>180+TL&&line.get_azimuth()<360-TL)){
                        if((line.get_azimuth()<180-TL&&line.get_azimuth()>TL)){
                            mm=180-TLM;
                        } else {
                            mm=180+TLM;
                        }
                    }
                    break;
                }

                //while文の弊害たち
                gam.read_azimuth();
                ball.read();
                line.read();

                //動きます
                mymotor.run(mm,(int)mt,0);
                mt+=3;//だんだん早くなる♪

            }
        } else {//1.5倍過ぎたら止めましょうと　誤爆防止や
            SilentTime.reset();
        }
    }
    }();

    if (line.get_type() != 0) {// === ラインあり ===
        frog = 1;//フ　ラ　グ　付　け

        // --- 次回用保存処理 ---
        [this](){lastdetect = line.get_azimuth();}();

        if (ball.get_stat() == 1) {// === ボールあり ===
            bool hv = abs(line.get_x())<2?true:false;//horizontal vertical
            if((hv||line.get_type()==2)&&tl==false){//水平ラインでの処理や(角も含む)
            mybuzzer.stop();
            ball_ang=ball.get_azimuth()<180?90:270;//右か左か

            rad = radians(line.get_azimuth());
            line_x = sin(rad);
            line_y = cos(rad);

            rad=radians(ball_ang);
            ball_x= sin(rad);
            ball_y= cos(rad);

            if(getErr(0,ball.get_azimuth())<ball_move_border){//目の前にあるときにボールを追いかける必要はない
                ball_x= 0;
                ball_y= 0;
            }

            move_x=(line_x+ball_x)*ball_power;
            move_y=((line_y*line_late)+(ball_y*ball_late))*ball_power;
            move_azimuth = myvector.get_azimuth(move_x, move_y);
            move_power = myvector.get_magnitude(abs(move_x), move_y);
            } else {
                mybuzzer.start(2000,999);
                ball_ang = (ball.get_azimuth()<90||ball.get_azimuth()>270) ? 0:180;
                line_ang = line.get_azimuth()<180?90:270;

                rad = radians(line.get_azimuth());
                line_x = sin(rad);
                line_y = cos(rad);

                rad=radians(ball_ang);
                ball_x= sin(rad);
                ball_y= cos(rad);

                // if(getErr(90,ball.get_azimuth())<ball_move_border||getErr(270,ball.get_azimuth())<ball_move_border){//目の前にあるときにボールを追いかける必要はない
                //     ball_x= 0;
                //     ball_y= 0;
                // }

                move_x=(line_x+ball_x)*ball_power;
                move_y=((line_y*line_late)+(ball_y*ball_late))*ball_power;
                move_azimuth = myvector.get_azimuth(move_x, move_y);
                move_power = myvector.get_magnitude(abs(move_x), abs(move_y));
            }


            if (move_power > move_border) {
                mymotor.run(move_azimuth, (int)move_power, 0);
            } else {
                mymotor.free();
                frog=6;
            }
        } else {
            frog=3;
            // === ボールなし === ラインに戻る
            move_y = line.get_y() * 20;
            move_azimuth = myvector.get_azimuth(0, move_y);
            move_power = myvector.get_magnitude(abs(move_x), move_y);

            if (move_power > move_border) {
                mymotor.run(move_azimuth, (int)move_power, 0);
            } else {
                mymotor.free();
            }
            Dtimer.reset();
        }

    } else {
        // === ラインなし：戻り処理 ===
        mymotor.run(lastdetect, 200, 0);
        mybuzzer.start(1500, 999);
        frog=2;
    }
    applyUI(frog);
    if(frog!=6){SilentTime.reset();}
}

void Defense::reset() {
    resetUI();
    Dtimer.reset();
    timer_started = false;
    for (int i = 0; i < 5; i++) {
        ball_history[i] = ball.get_azimuth();
    }
}


// ===================================
// UI処理
// ===================================

void Defense::resetUI() {
    background.reset();
    P_line.reset();
    P_ball.reset();
    move_ang.reset();
    exMoveX.reset();
    exMoveY.reset();
    dash_timer.reset();

    // --- 背景 #39C5BB7F ---
    background.red = 57;
    background.green = 197;
    background.blue = 187;
    background.alpha = 0.5;

    // --- ライン #00C832BF ---
    P_line.red = 0;
    P_line.green = 200;
    P_line.blue = 50;
    P_line.alpha = 0.75;

    // --- ボール #FF0000BF ---
    P_ball.red = 255;
    P_ball.green = 0;
    P_ball.blue = 0;
    P_ball.alpha = 0.75;

    // --- 移動方向 #0000FFFF ---
    move_ang.red = 0;
    move_ang.green = 0;
    move_ang.blue = 255;
    move_ang.alpha = 0.75;

    // --- ダッシュタイマー #DCB4DCFF ---
    dash_timer.red = 220;
    dash_timer.green = 180;
    dash_timer.blue = 220;
    dash_timer.alpha = 1.0;

    // --- 追加移動X軸 #8bfff7ff ---
    exMoveX.red = 255;
    exMoveX.green = 255;
    exMoveX.blue = 0;
    exMoveX.alpha = 1.0;

    // --- 追加移動Y軸 #d0ff50ff ---
    exMoveY.red = 208;
    exMoveY.green = 255;
    exMoveY.blue = 80;
    exMoveY.alpha = 1.0;
}

void Defense::applyUI(int mode) {
    // --- alpha適用 ---
    background.applyAlpha();
    P_line.applyAlpha();
    P_ball.applyAlpha();
    move_ang.applyAlpha();
    exMoveX.applyAlpha();
    exMoveY.applyAlpha();
    dash_timer.applyAlpha();

    if (mode == 1) { // ノーマル
        // --- 背景 ---
        mypixel.multi(0, 15, background.red, background.green, background.blue);

        // // --- ライン ---
        // mypixel.closest(line.get_azimuth(), P_line.red, P_line.green, P_line.blue, 3);


        // --- ボール ---
        // if (ball.get_stat() != 0) {
        //     mypixel.closest(ball.get_azimuth(), P_ball.red, P_ball.green, P_ball.blue, 1);
        // }

        // --- 移動方向 ---
        if (move_power > move_border) {
            mypixel.closest(move_azimuth, move_ang.red, move_ang.green, move_ang.blue, 2);
        }

        int d=line_ang;
        mypixel.closest(d, P_line.red, P_line.green, P_line.blue, 3);

        int d2=ball_ang;
        mypixel.closest(d2, P_ball.red, P_ball.green, P_ball.blue, 3);

    } else if (mode == 2) { // ラインなし
        // --- 背景 ---
        mypixel.multi(0, 15, 255, 0, 0);

        // --- lastライン ---
        mypixel.closest(lastdetect, P_line.red, P_line.green, P_line.blue, 3);

    } else if (mode == 3) {
        mypixel.multi(0, 15, 255, 100, 255);
/*
    // } else if (mode == 4) { // 角
    //     // --- 背景 ---
    //     mypixel.multi(0, 15, background.red, background.green, background.blue);

    //     // --- ライン ---
    //     mypixel.closest(line.get_azimuth(), P_line.red, P_line.green, P_line.blue, 3);

    //     // --- ボール ---
    //     if (ball.get_stat() != 0) {
    //         mypixel.closest(ball.get_azimuth(), P_ball.red, P_ball.green, P_ball.blue, 1);
    //     }

    //     int d = ball.get_azimuth() < 180 ? 90 : 270;
    //     // --- 移動方向 ---
    //     if (move_power > move_border) {
    //         mypixel.closest(d, move_ang.red, move_ang.green, move_ang.blue, 2);
    //     }

    //     // --- 追加移動X軸 ---
    //     if (ex) {
    //         mypixel.closest(norm360(d + 180), exMoveX.red, exMoveX.green, exMoveX.blue, 2);
    //     }

    // } else if (mode == 5) { // 角(賭け)
    //     // --- 背景 ---
    //     mypixel.multi(0, 15, background.red, background.green, background.blue);
    //     int d = ball.get_azimuth();
    //     // --- 移動方向 ---
    //     if (move_power > move_border) {
    //         mypixel.closest(d, exMoveY.red, exMoveY.green, exMoveY.blue, 2);
    //     }
*/
    } else if (mode == 6) { // 移動量（サイレントタイマーメーター）
        // --- 背景 ---
        mypixel.multi(0, 15, background.red, background.green, background.blue);

        // --- サイレントタイマーのメーター表示 ---
        float percent = (float)SilentTime.read_milli() / (float)dash_border;
        if (percent > 1.0)
            percent = 1.0;

        int num_pixels = (int)(percent * 16);
        if (num_pixels > 0) {
            mypixel.multi(0, num_pixels - 1, dash_timer.red, dash_timer.green, dash_timer.blue);
        }
    }
}



/* for close
ガラクタ置き場　消したら恨む
            // if (ball.get_azimuth() < 180) {
            //     move_x = ball_power;
            // } else{

            //     move_x = -ball_power;
            // }

            // if (ball.get_azimuth() < ball_move_border||ball.get_azimuth()>(360-ball_move_border)) {
            //     move_x = 0;
            // }

            // // --- ライン(Y軸)処理 ---
            // move_y = ((line_max / line_r) * line.get_y()) * line_late;

            // // --- 角処理 ---
            // if (abs(line.get_x()) > 2||line.get_type()==2) {
            //     frog=4;
            //     ex=false;
            //     if (diff_signs(line.get_x(), move_x)) {
            //         // X軸はライン優先
            //         ex=true;
            //         move_x = ((150 / line_r) * line.get_x()) * line_late;
            //     }

            //     if (diff_signs(line.get_x(), ball.get_x())) {
            //         // ボール距離が大きくかつラインに近いなら賭ける
            //         if (ball.get_x() > exitCorner && line.get_magnitude() < (line_max / 3)) {
            //             frog=5;
            //             move_x = -ball_power;
            //             if (ball.get_azimuth() < 180) {
            //                 move_x = ball_power;
            //             }
            //             move_y = ball_power;
            //         }
            //     }
            // }

            // // --- 移動量計算 ---
            // move_azimuth = myvector.get_azimuth(move_x, move_y);
            // move_power = myvector.get_magnitude(abs(move_x), move_y);

            // // --- 移動実行 ---
            // if (abs(move_power) > move_border) {
            //     mymotor.run(move_azimuth, (int)move_power, 0);
            // } else {
            //     frog=6;
            //     mymotor.free();
            // }
            // Dtimer.reset();
for close end */