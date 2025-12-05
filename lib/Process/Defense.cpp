#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"


//staticのやつら
constexpr uint_fast8_t Defense::edge_list[6];
int Defense::lastdetect[2] = {0};
int Defense::move_azimuth = 0;
float Defense::move_power = 0.0f;
double Defense::move_x = 0.0f;
double Defense::move_y = 0.0f;
int Defense::calc_move_speed = 0;
int Defense::lastpower = 999;
int Defense::last_x = 999;
int Defense::last_y = 999;
float Defense::rad = 0.0f;
float Defense::ball_ang = 0.0f;
float Defense::line_x = 0.0f;
float Defense::line_y = 0.0f;
float Defense::ball_x = 0.0f;
float Defense::ball_y = 0.0f;
Timer Defense::Dtimer;
Timer Defense::SilentTime;
Timer Defense::MoveTime;
Timer Defense::ReturnTime;
int Defense::calb = 0;
bool Defense::tl = false;
bool Defense::edge = false;
bool Defense::corner = false;
Defense::RGBA Defense::background;
Defense::RGBA Defense::P_line;
Defense::RGBA Defense::P_ball;
Defense::RGBA Defense::move_ang;
Defense::RGBA Defense::dash_timer;
int Defense::ddddd = 0;

// === 入力キャッシュ変数の定義 ===
int Defense::line_azimuth_cache = 0;
int Defense::line_type_cache = 0;
int Defense::line_x_cache = 0;
int Defense::line_y_cache = 0;
int Defense::ball_azimuth_cache = 0;
bool Defense::ball_stat_cache = false;
int Defense::gam_azimuth_cache = 0;

void Defense::setup() {
    reset();//初期化　その他は何にもない
    //...あれれ？　意味ないじゃんけ
}

void Defense::defense_() {
    // === 1. 入力データ取得・キャッシュ ===
    resetUI();
    readSensorData();
    
    // === 2. ダッシュ判定（最優先） ===
    if(checkDashCondition()) {
        dash();
        return;
    }
    
    // === 3. フラグ決定 ===
    determineFrog();
    
    // === 4. フラグに応じた動作実行 ===
    switch(frog) {
        case FROG::NORMAL:
            normal();
            break;
        case FROG::NO_LINE:
            noline();
            break;
        case FROG::NO_BALL:
            noball();
            break;
        case FROG::STOP:
            stop();
            break;
        case FROG::DASH:
            // ダッシュ処理は上で実行済み
            break;
        default:
            mymotor.free();
            break;
    }
    
    // === 5. UI更新 ===
    applyUI(static_cast<int>(frog));
    
    // === 6. タイマー処理 ===
    updateTimers();
}

void Defense::readSensorData() {
    // センサーデータを一度だけ読み取ってキャッシュ
    line_azimuth_cache = line.get_azimuth();
    line_type_cache = line.get_type();
    line_x_cache = line.get_x();
    line_y_cache = line.get_y();
    ball_azimuth_cache = ball.get_azimuth();
    ball_stat_cache = ball.get_stat();
    gam_azimuth_cache = gam.get_azimuth();
    
    // 共通計算
    calb = 0 - gam_azimuth_cache;
    
    // 縦ライン判定
    tl = false;
    bool frog1 = line.get_stat(0) || line.get_stat(1) || line.get_stat(2) ||
                line.get_stat(23) || line.get_stat(22);
    bool frog2 = line.get_stat(11) || line.get_stat(12) || line.get_stat(13) ||
                line.get_stat(10) || line.get_stat(9);
    if(frog1 && frog2) {
        tl = true;
        background = RGBA{255, 0, 255, 1};
    }

    // エッジ判定
    edge = line.get_stat(5) || line.get_stat(6) || line.get_stat(7) ||
           line.get_stat(17) || line.get_stat(18) || line.get_stat(19);

    corner =(line.get_type()==2&&(getErr(line.get_pack(0),line.get_pack(1))<110));
}

bool Defense::checkDashCondition() {
    return (SilentTime.read_milli() > dash_border && USE_DASH == true);
}

void Defense::determineFrog() {
    frog = FROG::NONE;

    if(line_type_cache == 0) {
        // ライン無し
        frog = FROG::NO_LINE;
    } else if(line_type_cache == 3) {
        // 角ライン（特殊処理）
        frog = FROG::NORMAL; // 仮
    } else if(!ball_stat_cache) {
        // ボール無し
        frog = FROG::NO_BALL;
    } else {
        // 通常モード
        frog = FROG::NORMAL;
    }
}


void Defense::dash() {
    frog = FROG::DASH;
    float TL = 20.0;
    float TLM = 60.0;

    if(SilentTime.read_milli() < dash_border * 1.2) {
        if(myswitch.check_toggle() == 0) {
            SilentTime.reset();
            return;
        }

        mypixel.multi(0, 15, 255, 50, 50);
        mypixel.show();
        SilentTime.reset();

        // 前進
        while(SilentTime.read_milli() < 300) {
            gam.read_azimuth();
            mymotor.run(0, 200, 0);
            if(myswitch.check_toggle() == 0) {
                SilentTime.reset();
                break;
            }
        }
        SilentTime.reset();

        // アタック
        while(SilentTime.read_milli() < dash_time) {
            gam.read_azimuth();
            ball.read();
            line.read();
            if(line.get_type() != 0) {
                lastdetect[0] = line.get_azimuth();
                lastdetect[1] = gam.get_azimuth();
            }
            attack.attack_();
            if(myswitch.check_toggle() == 0) {
                SilentTime.reset();
                break;
            }
        }
        SilentTime.reset();

        // 後退
        while(SilentTime.read_milli() < 300) {
            gam.read_azimuth();
            mymotor.run(180, 200, 0);
            if(myswitch.check_toggle() == 0) {
                SilentTime.reset();
                break;
            }
        }
        SilentTime.reset();

        // 復帰
        int mm = 180;
        int mt = 75;
        while(1) {
            if(line.get_type() == 1) {
                if((line.get_azimuth() < 180 - TL && line.get_azimuth() > TL) ||
                    (line.get_azimuth() > 180 + TL && line.get_azimuth() < 360 - TL)) {
                    if(line.get_azimuth() < 180 - TL && line.get_azimuth() > TL) {
                        mm = 180 - TLM;
                    } else {
                        mm = 180 + TLM;
                    }
                }
                break;
            }
            gam.read_azimuth();
            ball.read();
            line.read();
            mymotor.run(mm, static_cast<int>(mt), 0);
            if(mt < 120) mt += 1;
            if(myswitch.check_toggle() == 0) {
                SilentTime.reset();
                break;
            }
        }
        mymotor.brake();
        delay(100);
    } else {
        SilentTime.reset();
    }
}
void Defense::normal() {
    if (line.get_type() != 0) {// === ラインあり ===
        if(!(line.get_type()>=3)){
            frog = FROG::NORMAL;//フ　ラ　グ　付　け

            // --- 次回用保存処理 ---
            lastdetect[0]=line.get_azimuth();
            lastdetect[1]=gam.get_azimuth();

            if (/*ball.get_stat() == 1*/1) {// === ボールあり ===
                calb = 0 - gam.get_azimuth(); //進行方向補正

                //line
                int line_az = line.get_azimuth();
                rad = radians(line_az); //ラインに対しての戻る力
                line_x = sin(rad);
                line_y = cos(rad);
                //---

                //ball
                int ball_az = ball.get_azimuth();
                ball_ang = ball_az + ball_cal; //ボールの方向

                ball_y = (ball_ang < 90 || ball_ang > 270) ? 1 : -1; //0か1か
                ball_x = (ball_ang < 180) ? 1 : -1;
                //---

                //減算　縦か角
                int line_x_val = line.get_x();
                calc_move_speed = (line_x_val > 3 || tl) ? static_cast<int>(move_speed) >> 1 : move_speed; //速度減算
                // calc_move_speed=move_speed;
                //---

                //x
                if(tl)
                    ball_x = 0;
                //縦ラインならballｘは0にしておく

                if(corner){
                    line_x*=1.5;
                }
                move_x = ((line_x) + (ball_x)) * calc_move_speed;

                if(tl&&abs(line_x)<2)
                    move_x = 0;
                //縦ラインでの速度上昇用

                //---

                //y
                if(corner){
                    line_y*=1.5;
                }else{
                    if(!tl && abs(line_x_val) < 2)
                        ball_y = 0;

                    if(tl)
                        line_y = 0;
                    //縦ラインじゃなかったらballｙは0にしておく
                }

                move_y = ((line_y) + (ball_y)) * calc_move_speed;

                if((!tl)&&abs(line.get_y())<2)
                    move_y/=2;
                //並行ラインでの処理
                //---


                //計算
                move_azimuth = myvector.get_azimuth(move_x, move_y);
                move_power = myvector.get_magnitude(abs(move_x), abs(move_y));
                //---

                if(getErr(0,ball.get_azimuth())<ball_move_border&&!tl){
                    //縦ラインじゃないかつボールが正面に近ければ止まる
                    move_power=0;
                }

                if (move_power > move_border ) {
                    // if(diff_signs(last_x, move_x)&&lastpower>(move_border*1.2)){
                    //     mymotor.brake();
                    //     mypixel.multi(0, 15, 255, 255, 0);
                    // }

                    mymotor.run(move_azimuth, static_cast<int>(move_power), 0);
                    if(MoveTime.read_milli()>500){
                        SilentTime.reset();
                    }
                } else {
                    // if(lastpower>(move_border))
                    //     mymotor.brake();
                    // else
                    mymotor.free();

                    frog=FROG::STOP;
                    MoveTime.reset();
                }

                {//保存保存！
                    lastpower=static_cast<int>(move_power);
                    last_x=static_cast<int>(line_x);
                    last_y=static_cast<int>(line_y);
                }

            } else {
                frog=FROG::NO_BALL;
                // === ボールなし === ラインに戻る
                mybuzzer.start(500,999);
                mymotor.free();
            }
        } else {
            mymotor.run(0,200,0);
        }
        ReturnTime.reset();
    } else {
        // === ラインなし：戻り処理 ===
        if(ReturnTime.read_milli()>1000)
            mymotor.run_non_stabilization(norm360(lastdetect[0]+(lastdetect[1]-gam.get_azimuth())), 230);
        else
            mymotor.run(norm360(lastdetect[0]+(lastdetect[1]-gam.get_azimuth())), 230, 0);

        frog=FROG::NO_LINE;//フ　ラ　グ　付　け
    }
    applyUI(static_cast<int>(frog));
    if(static_cast<int>(frog)!=4&&MoveTime.read_milli()>500){
        SilentTime.reset();
    }
    ddddd=Dtimer.read_milli();
    Serial.println(ddddd);
    Dtimer.reset();
}

void Defense::reset() {
    resetUI();
    Dtimer.reset();
    SilentTime.reset();
    MoveTime.reset();
}

void Defense::resetUI() {
    background.reset();
    P_line.reset();
    P_ball.reset();
    move_ang.reset();
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

}

// void Defense::normal() {
//     mybuzzer.stop();
    
//     // 次回用保存処理
//     lastdetect[0] = line_azimuth_cache;
//     lastdetect[1] = gam_azimuth_cache;

//     // line方向ベクトル計算
//     rad = radians(line_azimuth_cache);
//     line_x = sin(rad);
//     line_y = cos(rad);

//     // ball方向ベクトル計算
//     ball_ang = ball_azimuth_cache + ball_cal;
//     ball_y = (ball_ang < 90 || ball_ang > 270) ? 1 : -1;
//     ball_x = (ball_ang < 180) ? 1 : -1;

//     // 速度減算
//     calc_move_speed = (line_x_cache > 3 || tl) ? static_cast<int>(move_speed) >> 1 : move_speed;

//     // X軸移動量計算
//     if(tl) ball_x = 0;
//     move_x = (line_x + ball_x) * calc_move_speed;
//     if(tl && abs(line_x) < 2) move_x = 0;

//     // Y軸移動量計算
//     if(!tl && abs(line_x_cache) < 2) ball_y = 0;
//     if(tl) line_y = 0;
//     move_y = (line_y + ball_y) * calc_move_speed;
//     if((!tl) && abs(line_y_cache) < 2) move_y /= 2;

//     // 最終移動ベクトル計算
//     move_azimuth = myvector.get_azimuth(move_x, move_y);
//     move_power = myvector.get_magnitude(abs(move_x), abs(move_y));

//     // 停止判定
//     if(getErr(0, ball_azimuth_cache) < ball_move_border && !tl) {
//         move_power = 0;
//     }

//     // モーター制御
//     if (move_power > move_border) {
//         mymotor.run(move_azimuth, static_cast<int>(move_power), 0);
//         if(MoveTime.read_milli() > 500) {
//             SilentTime.reset();
//         }
//     } else {
//         mymotor.free();
//         frog = FROG::STOP;
//         MoveTime.reset();
//     }

//     // 保存
//     lastpower = static_cast<int>(move_power);
//     last_x = static_cast<int>(line_x);
//     last_y = static_cast<int>(line_y);
// }

void Defense::noline() {
    if(ReturnTime.read_milli() > 1000) {
        mymotor.run_non_stabilization(norm360(lastdetect[0] + (lastdetect[1] - gam.get_azimuth())), 230);
    } else {
        mymotor.run(norm360(lastdetect[0] + (lastdetect[1] - gam.get_azimuth())), 230, 0);
    }
}

void Defense::noball() {
    mybuzzer.start(500, 999);
    mymotor.free();
}

void Defense::stop() {
    mymotor.free();
}

// 重複定義を削除しました

void Defense::updateTimers() {
    if(static_cast<int>(frog) != 4 && MoveTime.read_milli() > 500) {
        SilentTime.reset();
    }

    if(line.get_type() != 0 && line.get_type() != 3) {
        ReturnTime.reset();
    }

    ddddd = Dtimer.read_milli();
    Serial.println(ddddd);
    Dtimer.reset();
}

void Defense::applyUI(int mode) {
    // --- alpha適用 ---
    background.applyAlpha();
    P_line.applyAlpha();
    P_ball.applyAlpha();;
    move_ang.applyAlpha();
    dash_timer.applyAlpha();

    if (mode == 1) { // ノーマル
        // --- 背景 ---
        mypixel.multi(0, 15, background.red, background.green, background.blue);

        if(line_x!=0||line_y!=0)mypixel.closest(roundToCardinal(myvector.get_azimuth(line_x,line_y)), P_line.red, P_line.green, P_line.blue, 3);

        // int d2=ball_ang;
        if(ball_x!=0||ball_y!=0)mypixel.closest(roundToCardinal(myvector.get_azimuth(ball_x,ball_y)), P_ball.red, P_ball.green, P_ball.blue, 3);

        //--- 移動方向 ---
        // if (move_power > move_border)
        //     mypixel.closest(roundToCardinal(move_azimuth+calb), move_ang.red, move_ang.green, move_ang.blue, 2);

    } else if (mode == 2) { // ラインなし
        // --- 背景 ---
        background.red = 255;
        background.green = 0;
        background.blue = 0;
        background.alpha = 1.0;
        background.applyAlpha();
        mypixel.multi(0, 15, background.red, background.green, background.blue);

        // --- lastライン ---
        mypixel.closest(lastdetect[0], P_line.red, P_line.green, P_line.blue, 3);

    } else if (mode == 3) {
        mypixel.multi(0, 15, 255, 100, 255);
    } else if (mode == 4) { // 移動量（サイレントタイマーメーター）
        // mybuzzer.start(200+(SilentTime.read_milli()/3)<2000?200+(SilentTime.read_milli()/3):2000,999);
        // --- 背景 ---
        mypixel.multi(0, 15, background.red, background.green, background.blue);

        // --- サイレントタイマーのメーター表示 ---
        float percent = static_cast<float>(SilentTime.read_milli()) / static_cast<float>(dash_border);//ダッシュまでの時間の何割か
        if (percent > 1.0)
            percent = 1.0;//意味はないんやけど最大値は100％

        int num_pixels = static_cast<int>(percent * 16);//int型変換と同時に切り捨て
        if (num_pixels > 0) {//あとは表示
            mypixel.multi(0, num_pixels - 1, dash_timer.red, dash_timer.green, dash_timer.blue);
        }
    }
}








// void Defense::setup(void) {

// }

// void Defense::reset() {
// }

// void Defense::defense_(void) {
//     ball_azimuth = ball.get_azimuth();//メソッド呼び出しの削減
//     mypixel.closest(ball_azimuth, 0, 0, 255, 1);
//     ball_stat = ball.get_stat();
//     line_azimuth = line.get_azimuth();
//     line_stat = line.get_type();
//     r_azimuth = gam.get_azimuth();
//     calb = 0-r_azimuth;

//     [this]() {
//         if(line_stat == 0) {
//             //ライン無し
//             frog = FROG::NO_LINE;
//             mybuzzer.start(500,999);
//             return;
//         } else if(ball_stat == false) {
//             //ボール無し
//             frog = FROG::NORMAL;
//             return;
//         } else {
//             //ノーマル
//             frog = FROG::NORMAL;
//             return;
//         }
//     }();

//     if(frog == FROG::NORMAL) {
//         mybuzzer.stop();
//         lastdetect = line_azimuth;
//         if(isFront(line_azimuth)) {
//             go_border[0] = line_azimuth;
//             go_border[1] = norm360(line_azimuth + 180);
//         } else {
//             go_border[0] = norm360(line_azimuth + 180); //-かも
//             go_border[1] = line_azimuth;
//         }
//         to_range(ball_azimuth, go_border[0], false);

//         int go_flag;
//         if(go_border[0] < ball_azimuth && ball_azimuth < go_border[1]) {
//             go_flag = 0; // 範囲内
//         } else {
//             go_flag = 1; // 範囲外
//         }

//         int go_ang = go_border[go_flag] + 90; // 境界に垂直な角度
//         norm360P(go_ang);

//         if(line.get_type() == 2) {//縦ライン脱出
//             if(getErr(0,line.get_pack(0))<100 && getErr(180,line.get_pack(1))<10) {//縦ライン
//                 if(getErr(ball_azimuth, line.get_pack(0)) < getErr(ball_azimuth, line.get_pack(1))) {
//                     go_ang = line.get_pack(0);
//                 } else {
//                     go_ang = line.get_pack(1);
//                 }
//             } else {
//                 // if(getErr(180,go_ang)<20&&getErr(line.get_pack(0),line.get_pack(1))>100){//真後ろは避ける
//                 //     go_ang+=180;
//                 // }
//             }
//         }

//         // go_angに向かって移動
//         norm360P(go_ang);
//         mymotor.run(go_ang+calb, 170, 0);
//     } else if(frog == FROG::NO_LINE) {
//         mymotor.run(lastdetect, return_power, 0);
//     } else if(frog == FROG::NO_BALL) {
//         mymotor.free();
//     }
// }
































































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

//     //     if(line_x!=0||line_yallｘは0にしておくlosest(roundToCardinal(myvector.get_azimuth(line_x,line_y)), P_line.red, P_line.green, P_line.blue, 3);

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








// // === 調整用定数 ===
    // //#define BALL_FILTER_OFF   // ボールフィルタを無効にする場合
    // #define USE_DASH true
    // static constexpr int ball_cal =-10;
    // static constexpr float dash_border = 5000.0;        // ダッシュ待ち時間
    // static constexpr float dash_time = 1200.0;          // ダッシュ時間
    // static constexpr int dash_border_ball = 2000;    // ボール検出でダッシュ待ち時間リセット
    // static constexpr float ball_move_border = 7.0;      // ボール移動境界(±角度)
    // static constexpr float move_speed = 300.0;          // 移動スピード（旧: ball_power）
    // static constexpr float move_border = 50.0;          // 移動最小値 -o判定に使う
    // static constexpr float line_late = 1.0;         // ライン反応倍率
    // static constexpr float y_late = 1.0;         // ライン反応倍率
    // static constexpr float x_late = 1.0;
    // static constexpr float ball_late = 1.0;         // ボール反応倍率
    // static constexpr float calblate_sir_late = 1.2;
    // static int ddddd; //デバッグ用

    // static constexpr uint_fast8_t edge_list[6] = {5,6,7,17,18,19};

    // // === 処理用変数 ===
    // static int lastdetect[2];                    // 最後検出方向
    // static int move_azimuth;                  // 移動方向
    // static float move_power;                  // 移動パワー
    // static double move_x;                     // X軸移動量
    // static double move_y;                     // Y軸移動量
    // static int calc_move_speed;             // 速度減算
    // static int lastpower;
    // static int last_x;
    // static int last_y;

    // static float rad;
    // static float ball_ang;                    // ボール回避方向
    // static float line_x;
    // static float line_y;
    // static float ball_x;
    // static float ball_y;
    // static Timer Dtimer;                      // ディフェンスタイマー
    // static Timer SilentTime;

    // static int calb;
    // static bool tl;
    // static bool edge;
    // static bool frog1;
    // static bool frog2;

    // /// @brief 角度を0, 90, 180, 270の最も近い値に丸める
    // /// @param angle 入力角度（0-359）
    // /// @return 最も近い基本方向（0, 90, 180, 270）
    // inline int roundToCardinal(int angle) {
    //     angle = norm360(angle);  // 0-359に正規化
        
    //     // 各方向との誤差を計算
    //     int err0 = getErr(angle, 0);
    //     int err90 = getErr(angle, 90);
    //     int err180 = getErr(angle, 180);
    //     int err270 = getErr(angle, 270);
        
    //     // 最小誤差の方向を返す
    //     int minErr = err0;
    //     int result = 0;
        
    //     if (err90 < minErr) {
    //         minErr = err90;
    //         result = 90;
    //     }
    //     if (err180 < minErr) {
    //         minErr = err180;
    //         result = 180;
    //     }
    //     if (err270 < minErr) {
    //         minErr = err270;
    //         result = 270;
    //     }
        
    //     return result;
    // }

    // // === UI用構造体 ===
    // struct RGBA {
    //     int red;
    //     int green;
    //     int blue;
    //     float alpha;

    //     void reset(){
    //         red=0;
    //         green=0;
    //         blue=0;
    //         alpha=0;
    //     }

    //     void applyAlpha(){
    //         red=red*alpha;
    //         green=green*alpha;
    //         blue=blue*alpha;
    //         alpha=1.0;
    //     }
    // };

    // // === UI用変数 ===
    // static RGBA background;                   // 背景色
    // static RGBA P_line;                       // ライン表示色
    // static RGBA P_ball;                       // ボール表示色
    // static RGBA move_ang;                     // 移動方向表示色
    // static RGBA dash_timer;                   // ダッシュタイマー色

    // enum class FROG : int {
    //     NONE = 0,
    //     NORMAL = 1,     // ノーマル
    //     NO_LINE = 2,    // ラインなし
    //     NO_BALL = 3,    // ボールなし
    //     STOP = 4, // 停止
    //     DASH = 5      // ダッシュ中
    // };
    // FROG frog;                          //フラグ　1ノーマル 2ラインなし 3ボールなし 4停止　5ダッシュ中

    // // === UI処理 ===

    // // UI色設定
    // void resetUI();
    // // UI表示実行
    // void applyUI(int mode);


//             {
    //             // 計算式表示用の変数準備
    //             double line_component_x = line_x * line_late * x_late * calc_move_speed;
    //             double ball_component_x = ball_x * ball_late * x_late * calc_move_speed;
    //             double line_component_y = line_y * line_late * y_late * calc_move_speed;
    //             double ball_component_y = ball_y * ball_late * y_late * calc_move_speed;
                
    //             printf_s_long(R"RAW(
    // =================================================================================================================
    // === センサー入力値 ===
    // line.get_azimuth()    : %d度
    // line.get_type()       : %d
    // line.get_x()          : %d
    // line.get_y()          : %d
    // line.get_magnitude()  : %d
    // ball.get_azimuth()    : %d度
    // ball.get_stat()       : %d
    // gam.get_azimuth()     : %d度
    
    // === フラグ ===
    // tl (縦ライン)         : %d
    // edge (エッジ)         : %d
    
    // === 定数パラメータ ===
    // ball_cal              : %d
    // move_speed            : %d.%02d
    // calc_move_speed       : %d
    // line_late             : %d.%02d
    // x_late                : %d.%02d
    // y_late                : %d.%02d
    // ball_late             : %d.%02d
    // ball_move_border      : %d.%02d
    // move_border           : %d.%02d
    
    // === 角度計算 ===
    // calb = 0 - gam.get_azimuth()
    //      = 0 - %d = %d
    
    // ball_ang = ball.get_azimuth() + ball_cal
    //          = %d + %d = %d.%02d度
    
    // rad = radians(line.get_azimuth())
    //     = radians(%d) = %d.%04d rad
    
    // === ライン方向ベクトル ===
    // line_x = sin(rad) = sin(%d.%04d)
    //        = %d.%04d
    // line_y = cos(rad) = cos(%d.%04d)
    //        = %d.%04d
    
    // === ボール方向判定 ===
    // ball_y = (ball_ang<90 || ball_ang>270) ? 1 : -1
    //        = (%d.%02d<90 || %d.%02d>270) ? 1 : -1
    //        = %d
    // ball_x = (ball_ang<180) ? 1 : -1
    //        = (%d.%02d<180) ? 1 : -1
    //        = %d
    
    // === 速度減算計算 ===
    // calc_move_speed = (line.get_x()>3 || tl) ? move_speed>>1 : move_speed
    //                 = (%d>3 || %d) ? %d>>1 : %d
    //                 = %d
    
    // === X軸移動量計算 ===
    // line_component_x = line_x * line_late * x_late * calc_move_speed
    //                  = %d.%04d * %d.%02d * %d.%02d * %d
    //                  = %d.%02d
    // ball_component_x = ball_x * ball_late * x_late * calc_move_speed
    //                  = %d * %d.%02d * %d.%02d * %d
    //                  = %d.%02d
    // move_x = line_component_x + ball_component_x
    //        = %d.%02d + %d.%02d
    //        = %d.%02d
    // [tl補正後] move_x = %d.%02d
    
    // === Y軸移動量計算 ===
    // line_component_y = line_y * line_late * y_late * calc_move_speed
    //                  = %d.%04d * %d.%02d * %d.%02d * %d
    //                  = %d.%02d
    // ball_component_y = ball_y * ball_late * y_late * calc_move_speed
    //                  = %d * %d.%02d * %d.%02d * %d
    //                  = %d.%02d
    // move_y = line_component_y + ball_component_y
    //        = %d.%02d + %d.%02d
    //        = %d.%02d
    // [補正後] move_y = %d.%02d
    
    // === 最終移動ベクトル ===
    // move_azimuth = myvector.get_azimuth(move_x, move_y)
    //              = myvector.get_azimuth(%d.%02d, %d.%02d)
    //              = %d度
    // move_power = myvector.get_magnitude(abs(move_x), abs(move_y))
    //            = myvector.get_magnitude(%d.%02d, %d.%02d)
    //            = %d.%02d
    
    // === 停止判定 ===
    // getErr(0+ball_cal, ball.get_azimuth()) < ball_move_border && !tl
    // = getErr(%d, %d) < %d.%02d && !%d
    // = %d < %d.%02d && !%d
    // = %d (停止判定: %s)
    
    // === モーター制御 ===
    // move_power > move_border: %d.%02d > %d.%02d = %s
    // 実行: %s
    
    // ========================================
    // )RAW",
    //             // センサー入力値
    //             line.get_azimuth(), line.get_type(),
    //             static_cast<int>(line.get_x()), static_cast<int>(line.get_y()),
    //             static_cast<int>(line.get_magnitude()),
    //             ball.get_azimuth(), ball.get_stat(),
    //             gam.get_azimuth(),
                
    //             // フラグ
    //             tl, edge,
                
    //             // 定数パラメータ
    //             ball_cal,
    //             static_cast<int>(move_speed), static_cast<int>(move_speed * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(line_late), static_cast<int>(line_late * 100) % 100,
    //             static_cast<int>(x_late), static_cast<int>(x_late * 100) % 100,
    //             static_cast<int>(y_late), static_cast<int>(y_late * 100) % 100,
    //             static_cast<int>(ball_late), static_cast<int>(ball_late * 100) % 100,
    //             static_cast<int>(ball_move_border), static_cast<int>(ball_move_border * 100) % 100,
    //             static_cast<int>(move_border), static_cast<int>(move_border * 100) % 100,
                
    //             // 角度計算
    //             gam.get_azimuth(), calb,
    //             ball.get_azimuth(), ball_cal,
    //             static_cast<int>(ball_ang), static_cast<int>(ball_ang * 100) % 100,
    //             line.get_azimuth(),
    //             static_cast<int>(rad), static_cast<int>(rad * 10000) % 10000,
                
    //             // ライン方向ベクトル
    //             static_cast<int>(rad), static_cast<int>(rad * 10000) % 10000,
    //             static_cast<int>(line_x), static_cast<int>(abs(line_x) * 10000) % 10000,
    //             static_cast<int>(rad), static_cast<int>(rad * 10000) % 10000,
    //             static_cast<int>(line_y), static_cast<int>(abs(line_y) * 10000) % 10000,
                
    //             // ボール方向判定
    //             static_cast<int>(ball_ang), static_cast<int>(ball_ang * 100) % 100,
    //             static_cast<int>(ball_ang), static_cast<int>(ball_ang * 100) % 100,
    //             static_cast<int>(ball_y),
    //             static_cast<int>(ball_ang), static_cast<int>(ball_ang * 100) % 100,
    //             static_cast<int>(ball_x),
                
    //             // 速度減算計算
    //             static_cast<int>(line.get_x()), tl,
    //             static_cast<int>(move_speed), static_cast<int>(move_speed),
    //             calc_move_speed,
                
    //             // X軸移動量計算
    //             static_cast<int>(line_x), static_cast<int>(abs(line_x) * 10000) % 10000,
    //             static_cast<int>(line_late), static_cast<int>(line_late * 100) % 100,
    //             static_cast<int>(x_late), static_cast<int>(x_late * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(line_component_x), static_cast<int>(abs(line_component_x) * 100) % 100,
    //             static_cast<int>(ball_x),
    //             static_cast<int>(ball_late), static_cast<int>(ball_late * 100) % 100,
    //             static_cast<int>(x_late), static_cast<int>(x_late * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(ball_component_x), static_cast<int>(abs(ball_component_x) * 100) % 100,
    //             static_cast<int>(line_component_x), static_cast<int>(abs(line_component_x) * 100) % 100,
    //             static_cast<int>(ball_component_x), static_cast<int>(abs(ball_component_x) * 100) % 100,
    //             static_cast<int>(line_component_x + ball_component_x), static_cast<int>(abs(line_component_x + ball_component_x) * 100) % 100,
    //             static_cast<int>(move_x), static_cast<int>(abs(move_x) * 100) % 100,
                
    //             // Y軸移動量計算
    //             static_cast<int>(line_y), static_cast<int>(abs(line_y) * 10000) % 10000,
    //             static_cast<int>(line_late), static_cast<int>(line_late * 100) % 100,
    //             static_cast<int>(y_late), static_cast<int>(y_late * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(line_component_y), static_cast<int>(abs(line_component_y) * 100) % 100,
    //             static_cast<int>(ball_y),
    //             static_cast<int>(ball_late), static_cast<int>(ball_late * 100) % 100,
    //             static_cast<int>(y_late), static_cast<int>(y_late * 100) % 100,
    //             calc_move_speed,
    //             static_cast<int>(ball_component_y), static_cast<int>(abs(ball_component_y) * 100) % 100,
    //             static_cast<int>(line_component_y), static_cast<int>(abs(line_component_y) * 100) % 100,
    //             static_cast<int>(ball_component_y), static_cast<int>(abs(ball_component_y) * 100) % 100,
    //             static_cast<int>(line_component_y + ball_component_y), static_cast<int>(abs(line_component_y + ball_component_y) * 100) % 100,
    //             static_cast<int>(move_y), static_cast<int>(abs(move_y) * 100) % 100,
                
    //             // 最終移動ベクトル
    //             static_cast<int>(move_x), static_cast<int>(abs(move_x) * 100) % 100,
    //             static_cast<int>(move_y), static_cast<int>(abs(move_y) * 100) % 100,
    //             move_azimuth,
    //             static_cast<int>(abs(move_x)), static_cast<int>(abs(move_x) * 100) % 100,
    //             static_cast<int>(abs(move_y)), static_cast<int>(abs(move_y) * 100) % 100,
    //             static_cast<int>(move_power), static_cast<int>(move_power * 100) % 100,
                
    //             // 停止判定
    //             0 + ball_cal, ball.get_azimuth(),
    //             static_cast<int>(ball_move_border), static_cast<int>(ball_move_border * 100) % 100,
    //             tl,
    //             getErr(0 + ball_cal, ball.get_azimuth()),
    //             static_cast<int>(ball_move_border), static_cast<int>(ball_move_border * 100) % 100,
    //             tl,
    //             (getErr(0 + ball_cal, ball.get_azimuth()) < ball_move_border && !tl),
    //             (getErr(0 + ball_cal, ball.get_azimuth()) < ball_move_border && !tl) ? "停止" : "動作",
                
    //             // モーター制御
    //             static_cast<int>(move_power), static_cast<int>(move_power * 100) % 100,
    //             static_cast<int>(move_border), static_cast<int>(move_border * 100) % 100,
    //             (move_power > move_border) ? "true" : "false",
    //             (move_power > move_border) ? "mymotor.run()" : "mymotor.free()"
    //             );
    //             delay(100);  // シリアル出力の安定化のため待機
    //             }
    
    // #define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
// #define printf_s_long(fmt, ...) ({ char buf[3000]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })

