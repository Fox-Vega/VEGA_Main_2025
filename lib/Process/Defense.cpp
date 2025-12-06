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
Timer Defense::verticalTime;
int Defense::calb = 0;
bool Defense::tl = false;
bool Defense::corner = false;
bool Defense::frog1 = false;
bool Defense::frog2 = false;
Defense::RGBA Defense::background;
Defense::RGBA Defense::P_line;
Defense::RGBA Defense::P_ball;
Defense::RGBA Defense::move_ang;
Defense::RGBA Defense::dash_timer;
int Defense::ddddd = 0;
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
    frog1 = line.get_stat(0) || line.get_stat(1) || line.get_stat(2) ||
                line.get_stat(23) || line.get_stat(22);
    frog2 = line.get_stat(11) || line.get_stat(12) || line.get_stat(13) ||
                line.get_stat(10) || line.get_stat(9);
    if(frog1 && frog2) {
        tl = true;
        background = RGBA{255, 0, 255, 1};
    }

    // エッジ判定

    corner =(line.get_type()==2&&(getErr(line.get_pack(0),line.get_pack(1))<110));
    
    // === 2. ダッシュ判定（最優先） ===
    if((SilentTime.read_milli() > dash_border && USE_DASH == true)) {
        dash();
        return;
    }

    // === 3. フラグ決定 ===
    frog = FROG::NONE;

    if(line_type_cache == 0) {
        // ライン無し
        frog = FROG::NO_LINE;
    } else if(line_type_cache == 3) {
        // 角ライン（特殊処理）
        frog = FROG::BAD_LINE;
    } else if(!ball_stat_cache) {
        // ボール無し
        frog = FROG::NO_BALL;
    } else {
        // 通常モード
        frog = FROG::NORMAL;
    }

    // === 4. フラグに応じた動作実行 ===
    switch(frog) {
        case FROG::NORMAL:
            normal();
            break;
        case FROG::NO_LINE:
            noline();
            break;
        case FROG::BAD_LINE:
            // 3角ライン処理
            mybuzzer.start(1500, 999);
            mymotor.run(0,150,0);
            delay(vertical_return);
            verticalTime.reset();
            mybuzzer.stop();
            break;
        case FROG::NO_BALL:
            noball();
            break;
        case FROG::STOP:
            mymotor.free();
            break;
        case FROG::DASH:
            // ダッシュ処理は上で実行済み
            break;
        default:
            mymotor.free();
            break;
    }

    // === 5. UI更新 ===
    //applyUI(static_cast<int>(frog));

    // === 6. タイマー処理 ===
    int ddddd = Dtimer.read_milli();
    Serial.println(ddddd);
    updateTimers();
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
            attack.attack_();
            // mymotor.run(ball.get_azimuth(), 200, 0);
            // if(myswitch.check_toggle() == 0) {
            //     SilentTime.reset();
            //     break;
            // }
        }
        SilentTime.reset();
        mymotor.free();

        delay(10);//matuuuuuuuuuuuuu

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
            gam.read_azimuth();
            ball.read();
            line.read();
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
            mymotor.run(mm, static_cast<int>(mt), 0);
            if(mt < 120) mt += 1;
            if(myswitch.check_toggle() == 0) {
                SilentTime.reset();
                break;
            }
        }
        mymotor.free();
        delay(100);
    } else {
        SilentTime.reset();
    }
}


void Defense::normal() {
    mybuzzer.stop();
    
    // キャッシュを使用（メソッド呼び出し削減）
    lastdetect[0] = line_azimuth_cache;
    lastdetect[1] = gam_azimuth_cache;

    // 数学関数を一度だけ計算
    rad = radians(line_azimuth_cache);
    line_x = sin(rad);
    line_y = cos(rad);

    // ball方向ベクトル計算
    ball_ang = ball_azimuth_cache + ball_cal;
    ball_y = (ball_ang < 90 || ball_ang > 270) ? 1 : -1;
    ball_x = (ball_ang < 180) ? 1 : -1;

    // 速度減算（軽量化）
    calc_move_speed = (line_x_cache > 3 || tl) ? move_speed / 2 : move_speed;

    // X軸移動量計算
    if(tl) ball_x = 0;
    if(corner) line_x *= 1.5;
    
    move_x = (line_x + ball_x) * calc_move_speed;
    if(tl && abs(line_x) < 2) move_x = 0;

    // Y軸移動量計算
    if(corner) {
        line_y *= 1.5;
    } else {
        if(!tl && abs(line_x_cache) < 2) ball_y = 0;
        if(tl) line_y = 0;
    }
    
    move_y = (line_y + ball_y) * calc_move_speed;
    if((!tl) && abs(line_y_cache) < 2) move_y /= 2;

    // 最終移動ベクトル計算
    move_azimuth = myvector.get_azimuth(move_x, move_y);
    move_power = myvector.get_magnitude(abs(move_x), abs(move_y));

    // 停止判定
    if(getErr(0, ball_azimuth_cache) < ball_move_border && !tl) {
        move_power = 0;
    }

    // モーター制御
    if (move_power > move_border && (!tl || (tl&&isFront(ball_azimuth_cache)))) {
        mymotor.run(move_azimuth, (int)move_power, 0);  // 軽量キャスト
        if(MoveTime.read_milli() > 300) {
            SilentTime.reset();
        }
    } else {
        if(tl){
            SilentTime.reset();
        }
        mymotor.free();
        frog = FROG::STOP;
        MoveTime.reset();
        if(!tl)mybuzzer.start((int)scaleRange(0.0f, dash_border, 500.0f, 2000.0f, (float)SilentTime.read_milli()), 999);
    }

    // 保存
    lastpower = (int)move_power;
    last_x = (int)line_x;
    last_y = (int)line_y;
}



void Defense::reset() {
    resetUI();
    Dtimer.reset();
    SilentTime.reset();
    MoveTime.reset();
    ReturnTime.reset();
    verticalTime.reset();
}


void Defense::resetUI() {
    // background.reset();
    // P_line.reset();
    // P_ball.reset();
    // move_ang.reset();
    // dash_timer.reset();

    // // --- 背景 #39C5BB7F ---
    // background.red = 57;
    // background.green = 197;
    // background.blue = 187;
    // background.alpha = 0.5;

    // // --- ライン #00C832BF ---
    // P_line.red = 0;
    // P_line.green = 200;
    // P_line.blue = 50;
    // P_line.alpha = 0.75;

    // // --- ボール #FF0000BF ---
    // P_ball.red = 255;
    // P_ball.green = 0;
    // P_ball.blue = 0;
    // P_ball.alpha = 0.75;

    // // --- 移動方向 #0000FFFF ---
    // move_ang.red = 0;
    // move_ang.green = 0;
    // move_ang.blue = 255;
    // move_ang.alpha = 0.75;

    // // --- ダッシュタイマー #DCB4DCFF ---
    // dash_timer.red = 220;
    // dash_timer.green = 180;
    // dash_timer.blue = 220;
    // dash_timer.alpha = 1.0;

}

void Defense::noline() {
    if(ReturnTime.read_milli()>1000){
        mymotor.stabilization(0);
        mymotor.run(norm360(lastdetect[0]+(lastdetect[1]-gam.get_azimuth())), 230,0);
        mymotor.stabilization(1);
    }
    else
        mymotor.run(norm360(lastdetect[0]+(lastdetect[1]-gam.get_azimuth())), 230, 0);
}

void Defense::noball() {
    mybuzzer.start(500, 999);
    mymotor.free();
}


void Defense::updateTimers() {
    if(static_cast<int>(frog) != 4 && MoveTime.read_milli() > 500) {
        SilentTime.reset();
    }

    if(line.get_type() != 0 && line.get_type() != 3) {
        ReturnTime.reset();
    }

    if(!tl){
        verticalTime.reset();
    }

    Dtimer.reset();
}

void Defense::applyUI(int mode) {
    // // --- alpha適用 ---
    // background.applyAlpha();
    // P_line.applyAlpha();
    // P_ball.applyAlpha();;
    // move_ang.applyAlpha();
    // dash_timer.applyAlpha();

    // if (mode == 1) { // ノーマル
    //     // --- 背景 ---
    //     mypixel.multi(0, 15, background.red, background.green, background.blue);

    //     if(line_x!=0||line_y!=0)mypixel.closest(roundToCardinal(myvector.get_azimuth(line_x,line_y)), P_line.red, P_line.green, P_line.blue, 3);

    //     // int d2=ball_ang;
    //     if(ball_x!=0||ball_y!=0)mypixel.closest(roundToCardinal(myvector.get_azimuth(ball_x,ball_y)), P_ball.red, P_ball.green, P_ball.blue, 3);

    //     //--- 移動方向 ---
    //     // if (move_power > move_border)
    //     //     mypixel.closest(roundToCardinal(move_azimuth+calb), move_ang.red, move_ang.green, move_ang.blue, 2);

    // } else if (mode == 2) { // ラインなし
    //     // --- 背景 ---
    //     background.red = 255;
    //     background.green = 0;
    //     background.blue = 0;
    //     background.alpha = 1.0;
    //     background.applyAlpha();
    //     mypixel.multi(0, 15, background.red, background.green, background.blue);

    //     // --- lastライン ---
    //     mypixel.closest(lastdetect[0], P_line.red, P_line.green, P_line.blue, 3);

    // } else if (mode == 3) {
    //     mypixel.multi(0, 15, 255, 100, 255);
    // } else if (mode == 4) { // 移動量（サイレントタイマーメーター）
    //     // mybuzzer.start(200+(SilentTime.read_milli()/3)<2000?200+(SilentTime.read_milli()/3):2000,999);
    //     // --- 背景 ---
    //     mypixel.multi(0, 15, background.red, background.green, background.blue);

    //     // --- サイレントタイマーのメーター表示 ---
    //     float percent = static_cast<float>(SilentTime.read_milli()) / static_cast<float>(dash_border);//ダッシュまでの時間の何割か
    //     if (percent > 1.0)
    //         percent = 1.0;//意味はないんやけど最大値は100％

    //     int num_pixels = static_cast<int>(percent * 16);//int型変換と同時に切り捨て
    //     if (num_pixels > 0) {//あとは表示
    //         mypixel.multi(0, num_pixels - 1, dash_timer.red, dash_timer.green, dash_timer.blue);
    //     }
    //}
}
