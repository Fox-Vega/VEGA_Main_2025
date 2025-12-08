#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"\

//staticをクラス内で宣言したやつをcppで使えるように定義
//(クラス外で定義すればこんな事にならなかっったはず...)
const int Defense::dash_border;
const int Defense::move_speed;
const int Defense::move_border;
const int Defense::ball_cal;

void Defense::setup(void){
    //なにもない
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
    /// @note あんま信用ならん
    corner =(line.get_type()==2&&(getErr(line.get_pack(0),line.get_pack(1))<110));

    if(LastSilent-millis()>dash_border){//ダッシュ
        frog=FROG::DASH;//フラグ
        dash();//ダッシュして
        return;//抜ける
    }

    if(line_type==0){//ライン無し
        frog=FROG::NO_LINE;
        mybuzzer.start(1500,999);//ビィーーー
        mymotor.run(lastdetect,200,0);//最後のラインの方向に向かう
        return;
    }
    if(line_type==3){//角ライン
        frog=FROG::BAD_LINE;
        mymotor.run(0,200,0);//前進
        return;
    }
    if(ball_stat==false){//ボール無し
        frog=FROG::NO_BALL;
        mymotor.free();//空転
        mybuzzer.start(500,999);//プー
        return;
    }
    //通常動作
    frog=FROG::NORMAL;
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

    if(tl||corner){//縦ラインorコーナー
        calc_move_speed = move_speed>>1;//速度半分
    }

}

void Defense::dash(void){

}

void Defense::reset(void){
    //なにもない
}

