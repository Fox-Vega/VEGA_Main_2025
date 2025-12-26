#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"
inline void ToggleReturn() {if(myswitch.check_toggle() == 0) return;}
// グローバル変数の定義（Defense.cppでのみ定義）
Timer DashTimer,MoveTime,VeticalTime;
int lastdetect = 180;
int ball_flagged = 0;
int ball_flag = 0;
//-----調整用定数-----//
/// @brief ピクセル使用有無
#define Debug false
///　@brief ダッシュ待ち時間
constexpr unsigned long  dash_border = 2000;
/// @brief 基本移動速度
constexpr int move_speed = 200;
/// @brief 最小移動速度
const int move_border = 30;
/// @brief ボール補正角度
constexpr int ball_cal =0;
//// @brief ダッシュ時間
constexpr unsigned long dash_time = 400;
/// @brief ボール移動境界(±角度)
constexpr float ball_move_border = 2;
/// @brief ノイズ除去
constexpr unsigned long noise_border = 200;
/// @brief ライン強化
constexpr int line_back_mag=9;
/// @brief ライン強化2 ms
constexpr int line_least_times=100;
constexpr unsigned long Vetical_border=1500;
constexpr unsigned long Vetical_move_time=500;
constexpr int ball_far = 20;//これより大きかったら近いってことね
constexpr int back_ang[4] = {180, 180, 225, 135};
//-----調整できない定数-----//
constexpr int ball_x_calc=1;
#if Debug
int defense_hadling_time;
Timer Dhs;
#endif
/// @brief 初期化
void Defense::setup(void){reset();}
void Defense::defense_(int start_cord){
#if Debug
    Dhs.reset();
#endif

    if(start_cord != 0) {//開始座標が渡されたとき　{割り込み処理}
        while(!(line.get_type() == 2)) {//並行ラインまで待つ
            line.read();//ライン読み
            gam.read_azimuth();//これは多分姿勢制御に必要と見て読んでる
            start_cord = constrain(start_cord, 1, 4);//念のため範囲内に
            mymotor.run(back_ang[start_cord-1], 180, 0);//決められた角度に進むだけ　基本後ろか左後ろか右後ろ　特に特別な処理はない
            ToggleReturn();
        }
        mymotor.run(0,200,0);//少し前に進んでおく　前にいておけばなんとかる
        delay(100);//進む時間
        reset();
        return;
    }//こいつだけ特別に割り込み処理

    const int line_azimuth = line.get_azimuth();
    const int line_type = line.get_type();
    const int line_mag = line.get_magnitude();
    const int ball_azimuth = ball.get_azimuth();
    const int ball_stat = ball.get_stat();
    const int ball_intensity = ball.get_intensity();
    const int gam_azimuth = gam.get_azimuth();
    const unsigned long Dashtimer_milli = DashTimer.read_milli();
    if(Dashtimer_milli>(unsigned long)dash_border){
        if(!isFront(ball_azimuth)){
            reset();
            return;
        }
        if(Dashtimer_milli<dash_border+dash_time){
            mymotor.run(0, 220, 0);
            return;
        } else if(Dashtimer_milli>=dash_border+dash_time && Dashtimer_milli<dash_border+dash_time+50){
            mymotor.free();
            return;
        }else if(Dashtimer_milli>=dash_border+dash_time+50){
            if(line_type == 1) {
                reset();
                mymotor.run(0, 200, 0);
                delay(100);
                lastdetect=180;
                return;
            }
            mymotor.run(180, 150, 0);
            return;
        }
    }
    float move_power,move_x,move_y,ball_x,ball_y,line_x,line_y= 0.0f;//変数定義
    int calc_move_speed,move_azimuth = 0;
    // 縦ライン判定：前後のセンサ群が反応しているか（前7つ + 後ろ7つ）
    bool tl =(line.get_stat(23) || line.get_stat(0) || line.get_stat(1) || line.get_stat(2) || line.get_stat(3)) &&(line.get_stat(10) || line.get_stat(11) || line.get_stat(12) || line.get_stat(13) || line.get_stat(14));
    bool corner = line_type==2&&isDiagonalAngle(norm360(line.get_azimuth()+gam.get_azimuth()));
    bool edge = isInSide30(norm360(line_azimuth + gam_azimuth));//辺検知かつ前方30度以内
    const unsigned long VeticalTime_milli = VeticalTime.read_milli();
    if (VeticalTime_milli >= Vetical_border  && VeticalTime_milli < Vetical_border+Vetical_move_time) {
        if(ball_flagged==0){
            ball_flag = ball_azimuth < 180 ? 1 : -1;
            ball_flagged=1;
            return;
        }
        move_azimuth = norm360(myvector.get_azimuth(ball_flag,0));//ライン直角方向に向く
        mymotor.run(move_azimuth, 200, 0);
        return;
    } else if(VeticalTime_milli >= Vetical_border+Vetical_move_time && VeticalTime_milli < Vetical_border+Vetical_move_time+50){
        mymotor.free();
        return;
    }else if (VeticalTime_milli >= Vetical_border+Vetical_move_time+50){
        while(1){
            ToggleReturn();
            line.read();
            gam.read_azimuth();
            if(line.get_type() != 0) {
                reset();
                mymotor.run(0, 200, 0);
                delay(100);
                lastdetect=180;
                break;
            }
            else {
                mymotor.run(180, 190, 0);
            }
        }
        return;
    }else{
        ball_flagged=0;
    }
    if(line_type==0){//ラインないとき
        reset();
        int line_detect_time=0;
        while(line.get_type()==0){//ライン見えるまでループ
            ToggleReturn();
            line.read();//ライン読み
            gam.read_azimuth();//姿勢制御用
            mymotor.run(lastdetect,220,0);//最後の向きに行く
            if(!line.get_type()==0)line_detect_time=millis()+line_least_times;//ライン見えたら時間セット
            else if(line_detect_time!=0 && (int)millis()>line_detect_time)break;//ライン見えてから一定時間経ったら抜ける
        }
        return;
    }else if(line_type==3){//3点ライン
        reset();
        mymotor.run(0,200,0);//前行く
        return;
    }else if(ball_stat==false&&line_mag<5){//ボールない(見えない)
        reset();
        if(getErr(0,gam_azimuth)<10)mymotor.free();//動かない
        else mymotor.run(0,0,0);
        return;
    }else if(line_mag > line_back_mag){//ラインに戻る　アウト防止
        mymotor.run(line_azimuth, 200, 0);
    }else{
        lastdetect=line_azimuth;//最後の角度を保存
        calc_move_speed = corner ? move_speed>>1 : move_speed;//速度減算(1/2)
        ball_x = (ball_azimuth < 180 ? ball_x_calc : -1 * ball_x_calc);//ボールのｘ 右左の01
        if(tl) ball_x = 0;
        ball_y = (isFront(ball_azimuth) ? ball_x_calc : -1 * ball_x_calc);//y　これも上下で01
        if(!tl) ball_y = 0;
        line_x = (sin(radians(line_azimuth)));//並行ラインならラインのxは無効
        if(!tl) line_x = 0;
        line_y = (cos(radians(line_azimuth)));//縦ラインならラインのyは無効
        if(tl) line_y = 0;
        if(line_mag<5) line_y = 0;
        move_x = (line_x + ball_x) * calc_move_speed;//移動x
        move_y = (line_y + ball_y) * calc_move_speed;//移動y
        //もしかしたらこれ/2する...?

        if(corner){//角処理
            applyXY(line_azimuth, line_x, line_y);//ライン直角方向ベクトル取得
            ball_x = ball_azimuth < 180 ? ball_x_calc : -1*ball_x_calc;//ボールのｘ 右左の01
            move_x = diff_signs(ball_x,line_x) ? 0 : diff(ball_x)*move_speed;//x成分打ち消し
            move_y = 0;
        }

        if((tl&&!corner)||(edge&&!corner)){//縦での脱出処理
            applyXY(line_azimuth, line_x, line_y);//ライン直角方向ベクトル取得
            move_x = line_mag<5 ? 0 : diff(line_x) * move_speed;//xを01で
            move_y = move_speed;//yは前に進むように
        } else {
            VeticalTime.reset();//縦ラインじゃなければリセット
        }
        move_azimuth=norm360(myvector.get_azimuth(move_x, move_y));//移動角度計算　もし角ならラインの直角方向(並行線の方向)に向きを補正でなければxy合成
        move_power= (((getErr(0, ball_azimuth) < ball_move_border)||(getErr(180, ball_azimuth) < 10)) && (line_mag<6) && (!tl)) ? 0 : myvector.get_magnitude(abs(move_x), abs(move_y));//移動力計算　ボールが中央に近ければ止まる出なければ合成
        if((move_power > move_border) && (!tl || (tl && isFront(ball_azimuth)))){//動く　条件は動く力が小さすぎないことと縦ラインでではない||縦ラインでも前にボールがあること
            mymotor.run(move_azimuth, static_cast<int>(move_power),0);//移動実行
            const unsigned long MoveTime_milli = MoveTime.read_milli();
            if(((MoveTime_milli > noise_border)&&!corner)||(((getErr(180, ball_azimuth) < 7)))||ball_intensity<ball_far) DashTimer.reset();//ノイズ待ち時間過ぎたらダッシュの待ち時間をリセット
        }else{
            if((tl && !corner)||(getErr(180, ball_azimuth) < 7)){DashTimer.reset();};//縦ラインならダッシュ待ちリセット　縦ラインでの暴発防止　まあ前に動くけども
            MoveTime.reset();//動いてないのでリセット
            mymotor.free();//動かない
        }
    }
#if Debug
    mypixel.multi(0,scaleRange(0.0f, dash_border, 0.0f, 15.0f, (float)Dashtimer_milli), 0, 255, 0);//ダッシュ待ち時間表示　デバッグ
    mypixel.shows();
    defense_hadling_time=Dhs.read_milli();//処理速度取得
#endif
}
#if Debug
int Defense::defense_hadling_timeget(void){return defense_hadling_time;}
#else
int Defense::defense_hadling_timeget(void){return 999;}
#endif


void Defense::reset(void){
    MoveTime.reset();
    DashTimer.reset();
    VeticalTime.reset();
}

/*
ガラクタ置き場
bool tl = (line.get_stat(0) || line.get_stat(1) || line.get_stat(2) || line.get_stat(3) || line.get_stat(23) || line.get_stat(22) || line.get_stat(21))&& (line.get_stat(9) || line.get_stat(10) || line.get_stat(11) || line.get_stat(12) || line.get_stat(13) || line.get_stat(14) || line.get_stat(15));
*/