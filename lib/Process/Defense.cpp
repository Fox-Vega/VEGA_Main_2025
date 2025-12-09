#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"

/**
 * @note UI使わんからいったんフラグ消した
 * あと何で高速化できるか...?
 */

//staticをクラス内で宣言したやつをcppで使えるように定義
//(クラス外で定義すればこんな事にならなかっったはず...)
const float Defense::dash_border;
const int Defense::move_speed;
const int Defense::move_border;
const int Defense::ball_cal;
const float Defense::ball_move_border;

void Defense::setup(void){
    reset();
}

void Defense::defense_(int start_cord){
    if(start_cord != 0) {//復帰の開始座標が渡された
        int aaa = back_ang[start_cord-1];//方向決定
        mybuzzer.start(500,999);//ピー
        delay(500);//待つ
        while(!(line.get_type() == 2)) {//復帰検知まで繰り返す
            line.read();//whileの弊害
            ball.read();//whileの弊害
            gam.read_azimuth();//whileの弊害
            mymotor.run(aaa, 120, 0); //指定角度に向かう
            if(myswitch.check_toggle() == 0) {//終了処理
                return;
            }
        }
        mymotor.run(0,200,0);//ちょい前行く
        delay(100);//待つ
        return;
    }

        //-----キャッシュたち-----//

    /**
     * @brief line.get_azimuth()のキャッシュ
     * @ingroup キャッシュ変数
     */
    static int line_azimuth;
    /**
     * @brief line.get_type()のキャッシュ
     * @ingroup キャッシュ変数
     */
    int line_type;

    /**
     * @brief ball.get_azimuth()のキャッシュ
     * @ingroup キャッシュ変数
     */
    int ball_azimuth;

    /**
     * @brief ball.get_stat()のキャッシュ
     * @ingroup キャッシュ変数
     */
    int ball_stat;

    /**
     * @brief gam.get_azimuth()のキャッシュ
     * @ingroup キャッシュ変数
     */
    int gam_azimuth;

    //input系を取得
    line_azimuth=line.get_azimuth();
    line_type=line.get_type();
    ball_azimuth=ball.get_azimuth();
    ball_stat=ball.get_stat();
    gam_azimuth=gam.get_azimuth();

    // 縦ライン判定　機体の前と後ろのラインが同時反応で縦を検知
    tl =(line.get_stat(0) || line.get_stat(1) || line.get_stat(2) ||line.get_stat(23) || line.get_stat(22))&&
    (line.get_stat(11) || line.get_stat(12) || line.get_stat(13) || line.get_stat(10) || line.get_stat(9));
    // 角判定　2つのパック数かつその間の角度小さければみたいな感じ
    /// @note あんま信用ならん けど後でうまく行ってる
    corner =(line.get_type()==2&&(getErr(line.get_pack(0),line.get_pack(1))<110));

    if(LastSilent-millis()>dash_border){//ダッシュ
//        frog=FROG::DASH;//フラグ
        dash();//ダッシュして
        return;//抜ける
    }

    if(line_type==0){//ライン無し
//        frog=FROG::NO_LINE;
        mybuzzer.start(1500,999);//ビィーーー
        mymotor.run(lastdetect,200,0);//最後のラインの方向に向かう
        return;
    }
    if(line_type==3){//3角ライン
//        frog=FROG::BAD_LINE;
        mymotor.run(0,200,0);//前進
        return;
    }
    if(ball_stat==false){//ボール無し
//        frog=FROG::NO_BALL;
        mymotor.free();//空転
        mybuzzer.start(500,999);//プー
        return;
    }
    //通常動作
//    frog=FROG::NORMAL;
    mybuzzer.stop();//ブザー停止
    lastdetect=line_azimuth;//保存

    // lineのベクトル
    rad = radians(line_azimuth);
    line_x = sin(rad);
    line_y = cos(rad);

    // ballのベクトル (角度ベースで基本0か1かのような)
    ball_ang = ball_azimuth + ball_cal;
    ball_x = (ball_ang < 180) ? 1 : -1;//右か左か
    ball_y = (ball_ang < 90 || ball_ang > 270) ? 1 : -1;//前か後ろか

    if(tl||corner)//縦ラインorコーナー
        calc_move_speed = move_speed>>1;//速度半分

    //X計算----------

    /**
     * cornerは理論上離れたときも反応するから
     * それで言ったらこれライン強化としても動く
     * 運がいいな
    */
    if(corner) line_x *= 1.5;
    if(corner) line_y *= 1.5;

    if(tl) ball_x = 0;
    if(!tl) line_x = 0;

    move_x = (line_x + ball_x) * calc_move_speed;
    //ただの改善

    move_azimuth = myvector.get_azimuth(move_x, move_y);
    norm360P(move_azimuth);//保険
    move_power = myvector.get_magnitude(abs(move_x), abs(move_y));
    // 停止判定
    if(getErr(0, ball_azimuth) < ball_move_border && !tl) move_power = 0;

    if(move_power>move_border&&(!tl || (tl&&isFront(ball_azimuth)))){//動く条件
        mymotor.run(move_azimuth, (int)move_power, 0);
        if((LastMove-millis())>noise_border) LastSilent=millis();//サイレントタイマーリセット
    }else{
        if(!tl)LastSilent=millis();//サイレントタイマーリセット
        LastMove=millis();//ムーブタイマーリセット
        mymotor.free();//フリィィ
    }
}

void Defense::reset(void){
    LastMove=millis();
    LastSilent=millis();
}

void Defense::dash(void){
    LastMove=millis();//ムーブタイマーリセット
    
    float TL = 20.0;
    float TLM = 60.0;

    if((LastSilent - millis()) < dash_border * 1.2) {
        if(myswitch.check_toggle() == 0) {
            LastSilent = millis();
            return;
        }

        //mypixel.multi(0, 15, 255, 50, 50);
        mypixel.show();
        LastSilent = millis();
        
        // アタック
        int dash_start = millis();
        while((millis() - dash_start) < dash_time) {
            gam.read_azimuth();
            ball.read();
            line.read();

            mymotor.run(0, 220, 0);
            if(myswitch.check_toggle() == 0 || !isFront(ball.get_azimuth())) {
                LastSilent = millis();
                break;
            }
        }
        LastSilent = millis();
        mymotor.free();

        delay(10);//matuuuuuuuuuuuu

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
                LastSilent = millis();
                break;
            }
        }
        mymotor.free();
        delay(100);
    } else {
        LastSilent = millis();
    }
}