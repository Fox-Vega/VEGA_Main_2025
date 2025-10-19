#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"
// #define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
// #define printf_s_long(fmt, ...) ({ char buf[700]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
#define printf_s {}
#define printf_s_long {}
int Defense::move_azimuth = 0;
float Defense::move_power = 0.0f;
double Defense::move_x = 0.0;
double Defense::move_y = 0.0;
float Defense::rad = 0.0f;
float Defense::ball_ang = 0.0f;
float Defense::line_x = 0.0f;
float Defense::line_y = 0.0f;
float Defense::ball_x = 0.0f;
float Defense::ball_y = 0.0f;
int Defense::calb = 0;
Timer Defense::Dtimer;
Timer Defense::SilentTime;

void Defense::setup() {
    reset();//初期化　その他は何にもない
}

void Defense::defense_() {
    resetUI();
    frog = FROG::NONE; //フ　ラ　グ　付　け

    /*縦ライン*/
    tl=false;{bool frog1=false;bool frog2=false;int leng=4;for(int i=0; i<leng;i++){if(line.get_stat(i)==true){frog1=true;break;}}for(int i=1; i<leng;i++){if(line.get_stat(24-i)==true){frog1=true;break;}}for(int i=0; i<leng;i++){if(line.get_stat(11+i)==true){frog2=true;break;}}for(int i=1; i<leng;i++){if(line.get_stat(12-i)==true){frog2=true;break;}}if(frog1&&frog2){tl=true;background=RGBA{255,0,255,1};}}
    if(tl){printf_s("vertical line detected\n");}
    /*左右の端*/
    edge=false;{bool frog1=false;for (size_t i = 0; i < (sizeof(edge_list) / sizeof(edge_list[0])); i++) {if (line.get_stat(edge_list[i]) == true) {frog1 = true;break;}}if(frog1){edge=true;}}
    if(edge){printf_s("edge line detected\n");}

    // === ダッシュ処理 ===
    auto dash = [this]() -> void {if(SilentTime.read_milli()>dash_border&& USE_DASH == true){ //なんでラムダで囲んでるかって？ まあ気分だよ気分
        frog=FROG::DASH; //フ　ラ　グ　付　け
        float TL = 20.0;         //TL＝縦　ライン　(脳筋)
        float TLM = 60.0;        //TL＝縦　ライン　(脳筋)
        if(SilentTime.read_milli()<dash_border*1.2){//最初に起動防止　まあほぼ意味はないけど
            auto exitDash = [&]() -> bool {
                if(myswitch.check_toggle()==0){
                    return true;
                }
                return false;
            };
            if(exitDash()){SilentTime.reset(); return;} //ダッシュ中止

            mypixel.multi(0,15,255,50,50);mypixel.show();
            SilentTime.reset();

            //少し前に進みます
            while(SilentTime.read_milli()<300){gam.read_azimuth();mymotor.run(0,200,0);if(exitDash()){SilentTime.reset();break;}}SilentTime.reset();


            while(SilentTime.read_milli()<dash_time){//こちらがメイン　アタック呼び出してるだけ　頭悪い
                //while文の弊害たち
                gam.read_azimuth();
                ball.read();
                line.read();
                if(line.get_type()!=0){lastdetect[0]=line.get_azimuth();lastdetect[1]=gam.get_azimuth();} //更新
                attack.attack_();
                if(exitDash()){SilentTime.reset();break;}
            }SilentTime.reset();

            //少し後ろに進みます
            while(SilentTime.read_milli()<300){gam.read_azimuth();mymotor.run(180,200,0);if(exitDash()){SilentTime.reset();break;}}SilentTime.reset();

            int mm=180; //ムーブ向き　　ネーミングセンスは-100点
            int mt=75; //ムーブ力　　　名前の付け方頭悪い

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
                mymotor.run(mm,static_cast<int>(mt),0);
                if(mt<120)mt+=1;//だんだん早くなる♪

                if(exitDash()){SilentTime.reset();break;}
            }
            mymotor.brake();
            delay(100);
            return;//はじめに戻る
        } else {//1.5倍過ぎたら止めましょうと　誤爆防止や
            SilentTime.reset();
            return;//はじめに戻る
        }
    }
    };
    dash();

    if (line.get_type() != 0) {// === ラインあり ===
        frog = FROG::NORMAL;//フ　ラ　グ　付　け

        // --- 次回用保存処理 ---
        lastdetect[0]=line.get_azimuth();lastdetect[1]=gam.get_azimuth();

        if (ball.get_stat() == 1) {// === ボールあり ===
            calb=0-gam.get_azimuth();//進行方向補正

            rad = radians(line.get_azimuth());//ラインに対しての戻る力
            line_x = sin(rad);
            line_y = cos(rad);



            ball_ang=ball.get_azimuth()+ball_cal;//ボールの方向

            ball_y= (ball_ang<90||ball_ang>270)?1:-1;

            ball_x= (ball_ang<180)?1:-1;

            int calc_move_speed=move_speed;//速度減算
            if((tl||abs(line.get_x())>2)){
                mybuzzer.start(100,999);
                //calc_move_speed=calc_move_speed>>1;
            } else {
                mybuzzer.stop();
            }

            if(tl||abs(line.get_x())>2){ball_x=0;
            printf_s("vertical or edge ball x=0\n");}//縦ラインorエッジならｘは0にしておく
            move_x=((line_x*line_late*x_late)+(ball_x*ball_late*x_late))*calc_move_speed;
            if(tl&&abs(line.get_x())<3){move_x=0;
            printf_s("vertical move x=0\n");}//縦ライン

            if(!tl&&abs(line.get_x())<2){ball_y=0;
            printf_s("not vertical ball y=0\n");}//縦ラインじゃなかったらｙは0にしておく
            move_y=((line_y*line_late*y_late)+(ball_y*ball_late*y_late))*calc_move_speed;
            if(abs(line.get_y())<3&&!tl&&abs(line.get_x())<2){move_y=0;
            printf_s("not vertical and near line move y=0\n");}//並行ラインでの処理やったけど縦でおかしいからtl判定を有効にした

            move_azimuth = myvector.get_azimuth(move_x, move_y);
            move_power = myvector.get_magnitude(abs(move_x), abs(move_y));

            if(getErr(0+ball_cal,ball.get_azimuth())<ball_move_border&&!tl&&!(line.get_x()<3)){
                move_power=0;
            }
            if (move_power > move_border ) {
                if(diff_signs(move_x, last_x)||diff_signs(move_y, last_y)){
                    mymotor.brake();
                } else{
                mymotor.run(move_azimuth, static_cast<int>(move_power), 0);
            }
                SilentTime.reset();
            } else {
                if(move_power==0&&lastpower>100){
                    mymotor.brake();
                } else {
                    printf_s("motor free\n");
                    mymotor.free();
                    frog=FROG::STOP;
                }
            }

            lastpower=static_cast<int>(move_power);
            last_x=static_cast<int>(line_x);
            last_y=static_cast<int>(line_y);

        } else {
            frog=FROG::NO_BALL;
            // === ボールなし === ラインに戻る
            mybuzzer.start(500,999);
            mymotor.free();
            Dtimer.reset();
        }

    } else {
        // === ラインなし：戻り処理 ===
        mymotor.run_non_stabilization(norm360(lastdetect[0]+(lastdetect[1]-gam.get_azimuth())), 230);
        mybuzzer.start(1500, 999);
        frog=FROG::NO_LINE;//フ　ラ　グ　付　け
    }
    applyUI(static_cast<int>(frog));
    if(static_cast<int>(frog)!=4)
        SilentTime.reset();
}

void Defense::reset() {
    resetUI();
    Dtimer.reset();
    SilentTime.reset();
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

void Defense::applyUI(int mode) {
    // --- alpha適用 ---
    background.applyAlpha();
    P_line.applyAlpha();
    P_ball.applyAlpha();
    move_ang.applyAlpha();
    dash_timer.applyAlpha();

    if (mode == 1) { // ノーマル
        // --- 背景 ---
        mypixel.multi(0, 15, background.red, background.green, background.blue);

        mypixel.closest(line.get_azimuth(), P_line.red, P_line.green, P_line.blue, 3);

        // int d2=ball_ang;
        mypixel.closest(ball_ang, P_ball.red, P_ball.green, P_ball.blue, 3);

        //--- 移動方向 ---
        if (move_power > move_border)
            mypixel.closest(move_azimuth+calb, move_ang.red, move_ang.green, move_ang.blue, 2);

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
        // mybuzzer.start(200+(SilentTime.read_milli()/3),999);
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


/*
printf_s_long(R"RAW(
| ============================
| move_x: %d ;
| move_y: %d ;
| move_azimuth: %d ;
| mod : %d ;
| move_power: %d ;
| ball_ang: %d ;
| ball_x : %d ;
| ball_y : %d ;
| line_x : %d ;
| line_y : %d ;

| calb : %d ;

| X||%d+%d|| %d
| Y||%d+%d|| %d

| line_distance : %d ;
| line_y : %d ;
| line_x : %d ;

|line_x cal: %d/100;
| =============================


)RAW",
static_cast<int>(move_x),
static_cast<int>(move_y),
static_cast<int>(move_azimuth),
norm360(static_cast<int>(move_azimuth+calb)),
static_cast<int>(move_power),
static_cast<int>(ball_ang),
static_cast<int>(ball_x),
static_cast<int>(ball_y),
static_cast<int>(line_x),
static_cast<int>(line_y),
calb,
static_cast<int>(line_x),
static_cast<int>(ball_x),
static_cast<int>(line_x+ball_x*calc_move_speed),
static_cast<int>(line_y),
static_cast<int>(ball_y),
(static_cast<int>(line_y)+static_cast<int>(ball_y)*calc_move_speed),
static_cast<int>(abs(line.get_magnitude())),
static_cast<int>(line.get_y()),
static_cast<int>(line.get_x()),
static_cast<int>(line_x * 100)  // 小数点以下2桁を整数として表示
);
*/
