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

// #define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
// #define printf_s_long(fmt, ...) ({ char buf[3000]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })

// //staticのやつら
// constexpr uint_fast8_t Defense::edge_list[6];
// int Defense::lastdetect[2] = {0};
// int Defense::move_azimuth = 0;
// float Defense::move_power = 0.0f;
// double Defense::move_x = 0.0f;
// double Defense::move_y = 0.0f                                                                                                   ;
// int Defense::calc_move_speed = 0;
// int Defense::lastpower = 999;
// int Defense::last_x = 999;
// int Defense::last_y = 999;
// float Defense::rad = 0.0f;
// float Defense::ball_ang = 0.0f;
// float Defense::line_x = 0.0f;
// float Defense::line_y = 0.0f;
// float Defense::ball_x = 0.0f;
// float Defense::ball_y = 0.0f;
// Timer Defense::Dtimer;
// Timer Defense::SilentTime;
// int Defense::calb = 0;
// bool Defense::tl = false;
// bool Defense::edge = false;
// Defense::RGBA Defense::background;
// Defense::RGBA Defense::P_line;
// Defense::RGBA Defense::P_ball;
// Defense::RGBA Defense::move_ang;
// Defense::RGBA Defense::dash_timer;
// int Defense::ddddd = 0;

// void Defense::setup() {
//     reset();//初期化　その他は何にもない
//     //...あれれ？　意味ないじゃんけ
// }

// void Defense::defense_() {
//     resetUI();
//     frog = FROG::NONE; //フ　ラ　グ　付　け

//     /*縦ライン*/
//     tl = false;
//     bool frog1 =line.get_stat(0) || line.get_stat(1) || line.get_stat(2) ||
//                 line.get_stat(23) || line.get_stat(22);
//     bool frog2 =line.get_stat(11) || line.get_stat(12) || line.get_stat(13) ||
//                 line.get_stat(10) || line.get_stat(9);
//     if(frog1 && frog2) {
//         tl = true;
//         background = RGBA{255, 0, 255, 1};
//     }

//     /*左右の端*/
//     edge =  line.get_stat(5) || line.get_stat(6) || line.get_stat(7) ||
//             line.get_stat(17) || line.get_stat(18) || line.get_stat(19  );

//     //=== ダッシュ処理 ===
//     if(SilentTime.read_milli() > dash_border && USE_DASH == true) {
//         frog = FROG::DASH;
//         float TL = 20.0;  //TL＝縦　ライン　(脳筋)
//         float TLM = 60.0; //TL＝縦　ライン　(脳筋)

//         if(SilentTime.read_milli() < dash_border * 1.2) { //最初に起動防止
//             if(myswitch.check_toggle() == 0) {
//                 SilentTime.reset();
//                 return; //ダッシュ中止
//             }

//             mypixel.multi(0, 15, 255, 50, 50);
//             mypixel.show();
//             SilentTime.reset();

//             //少し前に進みます
//             while(SilentTime.read_milli() < 300) {
//                 gam.read_azimuth();
//                 mymotor.run(0, 200, 0);
//                 if(myswitch.check_toggle() == 0) {
//                     SilentTime.reset();
//                     break;
//                 }
//             }
//             SilentTime.reset();

//             //こちらがメイン　アタック呼び出してるだけ
//             while(SilentTime.read_milli() < dash_time) {
//                 gam.read_azimuth();
//                 ball.read();
//                 line.read();
//                 if(line.get_type() != 0) {
//                     lastdetect[0] = line.get_azimuth();
//                     lastdetect[1] = gam.get_azimuth();
//                 }
//                 attack.attack_();
//                 if(myswitch.check_toggle() == 0) {
//                     SilentTime.reset();
//                     break;
//                 }
//             }
//             SilentTime.reset();

//             //少し後ろに進みます
//             while(SilentTime.read_milli() < 300) {
//                 gam.read_azimuth();
//                 mymotor.run(180, 200, 0);
//                 if(myswitch.check_toggle() == 0) {
//                     SilentTime.reset();
//                     break;
//                 }
//             }
//             SilentTime.reset();

//             int mm = 180; //ムーブ向き
//             int mt = 75;  //ムーブ力

//             while(1) { //戻ろう
//                 if(line.get_type() == 1) { //縦ライン見えたら少しずらす
//                     if((line.get_azimuth() < 180 - TL && line.get_azimuth() > TL) ||
//                         (line.get_azimuth() > 180 + TL && line.get_azimuth() < 360 - TL)) {
//                         if(line.get_azimuth() < 180 - TL && line.get_azimuth() > TL) {
//                             mm = 180 - TLM;
//                         } else {
//                             mm = 180 + TLM;
//                         }
//                     }
//                     break;
//                 }

//                 gam.read_azimuth();
//                 ball.read();
//                 line.read();

//                 mymotor.run(mm, static_cast<int>(mt), 0);
//                 if(mt < 120) mt += 1; //だんだん早くなる♪

//                 if(myswitch.check_toggle() == 0) {
//                     SilentTime.reset();
//                     break;
//                 }
//             }
//             mymotor.brake();
//             delay(100);
//             return;
//         } else { //1.5倍過ぎたら止めましょうと　誤爆防止や
//             SilentTime.reset();
//             return;
//         }
//     }

//     if (line.get_type() != 0) {// === ラインあり ===
//         if(line.get_type()!=3){
//         frog = FROG::NORMAL;//フ　ラ　グ　付　け

//         // --- 次回用保存処理 ---
//         lastdetect[0]=line.get_azimuth();
//         lastdetect[1]=gam.get_azimuth();

//         if (/*ball.get_stat() == 1*/1) {// === ボールあり ===
//             calb = 0 - gam.get_azimuth(); //進行方向補正

//             //line
//             int line_az = line.get_azimuth();
//             rad = radians(line_az); //ラインに対しての戻る力
//             line_x = sin(rad);
//             line_y = cos(rad);
//             //---

//             //ball
//             int ball_az = ball.get_azimuth();
//             ball_ang = ball_az + ball_cal; //ボールの方向

//             ball_y = (ball_ang < 90 || ball_ang > 270) ? 1 : -1; //0か1か
//             ball_x = (ball_ang < 180) ? 1 : -1;
//             //---

//             //減算　縦か角
//             int line_x_val = line.get_x();
//             calc_move_speed = (line_x_val > 3 || tl) ? static_cast<int>(move_speed) >> 1 : move_speed; //速度減算
//             // calc_move_speed=move_speed;
//             //---

//             //x
//             if(tl)
//                 ball_x = 0;
//             //縦ラインならballｘは0にしておく

//             move_x = ((line_x * line_late * x_late) + (ball_x * ball_late * x_late)) * calc_move_speed;

//             if(tl&&abs(line_x)<2)
//                 move_x = 0;
//             //縦ラインでの速度上昇用

//             //---

//             //y
//             if(!tl && abs(line_x_val) < 2)
//                 ball_y = 0;

//             if(tl)
//                 line_y = 0;
//             //縦ラインじゃなかったらballｙは0にしておく

//             move_y = ((line_y * line_late * y_late) + (ball_y * ball_late * y_late)) * calc_move_speed;

//             if((!tl)&&abs(line.get_y())<2)
//                 move_y/=2;
//             //並行ラインでの処理
//             //---

//             //計算
//             move_azimuth = myvector.get_azimuth(move_x, move_y);
//             move_power = myvector.get_magnitude(abs(move_x), abs(move_y));
//             //---

//             if(getErr(0+ball_cal,ball.get_azimuth())<ball_move_border&&!tl){
//                 //縦ラインじゃないかつボールが正面に近ければ止まる
//                 move_power=0;
//             }

//             if (move_power > move_border ) {
//                     mymotor.run(move_azimuth, static_cast<int>(move_power), 0);
//                 SilentTime.reset();
//             } else {
//                     mymotor.free();
//                     frog=FROG::STOP;
//             }

//             {//保存保存！
//             lastpower=static_cast<int>(move_power);
//             last_x=static_cast<int>(line_x);
//             last_y=static_cast<int>(line_y);
//             }

//         } else {
//             frog=FROG::NO_BALL;
//             // === ボールなし === ラインに戻る
//             mybuzzer.start(500,999);
//             mymotor.free();
//         }
//     } else {
//         mymotor.run(0,200,0);
//     }
//     } else {
//         // === ラインなし：戻り処理 ===
//         mymotor.run(norm360(lastdetect[0]+(lastdetect[1]-gam.get_azimuth())), 230, 0);
//         //一応姿勢制御なし　まあ機体角度とか考慮してなんとか
//         frog=FROG::NO_LINE;//フ　ラ　グ　付　け
//     }
//     applyUI(static_cast<int>(frog));
//     if(static_cast<int>(frog)!=4){
//         SilentTime.reset();
//     }
//     ddddd=Dtimer.read_milli();
//     Serial.println(ddddd);
//     Dtimer.reset();
// }

// void Defense::reset() {
//     resetUI();
//     Dtimer.reset();
//     SilentTime.reset();
// }

// void Defense::resetUI() {
//     background.reset();
//     P_line.reset();
//     P_ball.reset();
//     move_ang.reset();
//     dash_timer.reset();

//     // --- 背景 #39C5BB7F ---
//     background.red = 57;
//     background.green = 197;
//     background.blue = 187;
//     background.alpha = 0.5;

//     // --- ライン #00C832BF ---
//     P_line.red = 0;
//     P_line.green = 200;
//     P_line.blue = 50;
//     P_line.alpha = 0.75;

//     // --- ボール #FF0000BF ---
//     P_ball.red = 255;
//     P_ball.green = 0;
//     P_ball.blue = 0;
//     P_ball.alpha = 0.75;

//     // --- 移動方向 #0000FFFF ---
//     move_ang.red = 0;
//     move_ang.green = 0;
//     move_ang.blue = 255;
//     move_ang.alpha = 0.75;

//     // --- ダッシュタイマー #DCB4DCFF ---
//     dash_timer.red = 220;
//     dash_timer.green = 180;
//     dash_timer.blue = 220;
//     dash_timer.alpha = 1.0;

// }

// void Defense::applyUI(int mode) {
//     // // --- alpha適用 ---
//     // background.applyAlpha();
//     // P_line.applyAlpha();
//     // P_ball.applyAlpha();
//     // move_ang.applyAlpha();
//     // dash_timer.applyAlpha();

//     // if (mode == 1) { // ノーマル
//     //     // --- 背景 ---
//     //     mypixel.multi(0, 15, background.red, background.green, background.blue);

//     //     if(line_x!=0||line_y!=0)mypixel.closest(roundToCardinal(myvector.get_azimuth(line_x,line_y)), P_line.red, P_line.green, P_line.blue, 3);

//     //     // int d2=ball_ang;
//     //     if(ball_x!=0||ball_y!=0)mypixel.closest(roundToCardinal(myvector.get_azimuth(ball_x,ball_y)), P_ball.red, P_ball.green, P_ball.blue, 3);

//     //     //--- 移動方向 ---
//     //     // if (move_power > move_border)
//     //     //     mypixel.closest(roundToCardinal(move_azimuth+calb), move_ang.red, move_ang.green, move_ang.blue, 2);

//     // } else if (mode == 2) { // ラインなし
//     //     // --- 背景 ---
//     //     background.red = 255;
//     //     background.green = 0;
//     //     background.blue = 0;
//     //     background.alpha = 1.0;
//     //     background.applyAlpha();
//     //     mypixel.multi(0, 15, background.red, background.green, background.blue);

//     //     // --- lastライン ---
//     //     mypixel.closest(lastdetect[0], P_line.red, P_line.green, P_line.blue, 3);

//     // } else if (mode == 3) {
//     //     mypixel.multi(0, 15, 255, 100, 255);
//     // } else if (mode == 4) { // 移動量（サイレントタイマーメーター）
//     //     mybuzzer.start(200+(SilentTime.read_milli()/3)<2000?200+(SilentTime.read_milli()/3):2000,999);
//     //     // --- 背景 ---
//     //     mypixel.multi(0, 15, background.red, background.green, background.blue);

//     //     // --- サイレントタイマーのメーター表示 ---
//     //     float percent = static_cast<float>(SilentTime.read_milli()) / static_cast<float>(dash_border);//ダッシュまでの時間の何割か
//     //     if (percent > 1.0)
//     //         percent = 1.0;//意味はないんやけど最大値は100％

//     //     int num_pixels = static_cast<int>(percent * 16);//int型変換と同時に切り捨て
//     //     if (num_pixels > 0) {//あとは表示
//     //         mypixel.multi(0, num_pixels - 1, dash_timer.red, dash_timer.green, dash_timer.blue);
//     //     }
//     // }
// }








