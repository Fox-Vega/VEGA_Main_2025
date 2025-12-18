#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"

///@brief　そゆこと
#define diff(a) a/abs(a)
constexpr int ball_x_calc=1;

// Timer変数の実体定義
Timer SilentTime,MoveTime;
int LastReturn = 0;

// 復帰方向リストの定義
const int back_ang[4] = {180, 180, 225, 135};

// グローバル変数の定義（Defense.cppでのみ定義）
static int lastdetect = 0;
static int last_exit_corner= 0;

//-----調整用定数-----//

///　@brief ダッシュ待ち時間
const int dash_border = 12000;
/// @brief 基本移動速度
const int move_speed = 200;
/// @brief 最小移動速度
const int move_border = 30;
/// @brief ボール補正角度
const int ball_cal =20;
//// @brief ダッシュ時間
const unsigned int dash_time = 2500;
/// @brief ボール移動境界(±角度)
const float ball_move_border = 4;
/// @brief ノイズ除去
const int noise_border = 400;
/// @brief ライン強化
const int line_back_mag=9;
/// @brief ライン強化2
const int line_powerup=1.8;

//処理速度
int dhst;
Timer Dhs;

/// @brief 初期化
void Defense::setup(void){
    reset();
}

void Defense::defense_(int start_cord){
    Dhs.reset();//処理速度リセット　ここから計測開始
    mypixel.use_pixel(true);//デバッグ
    if(start_cord != 0) {//開始座標が渡されたとき　{割り込み処理}
        mybuzzer.start(500,999);//ブー
        delay(100);//待つ　これは消していいかもしれないけどわかりやすいから残しておく
        while(!(line.get_type() == 2)) {//並行ラインまで待つ
            line.read();//ライン読み
            gam.read_azimuth();//これは多分姿勢制御に必要と見て読んでる
            mymotor.run(back_ang[start_cord-1], 120, 0);//決められた角度に進むだけ　基本後ろか左後ろか右後ろ　特に特別な処理はない
            if(myswitch.check_toggle() == 0) return;//トグルで終了させるための処理
        }
        mymotor.run(0,200,0);//少し前に進んでおく　前にいておけばなんとかる
        delay(100);//進む時間
        return;//関数を強制終了してその後はmainで座標がリセットされつ　つまりここが呼び出されることは一度以降ない
    }
    int line_azimuth,line_type,ball_azimuth,ball_stat,gam_azimuth,line_mag;//変数定義
    float move_power,move_x,move_y,ball_x,ball_y,line_x,line_y= 0.0f;//変数定義
    int calc_move_speed,ball_ang,move_azimuth = 0;//変数定義
    line_azimuth=line.get_azimuth();//キャッシュ
    line_type=line.get_type();//キャッシュ
    line_mag=line.get_magnitude();//キャッシュ
    ball_azimuth=ball.get_azimuth();//キャッシュ
    ball_stat=ball.get_stat();//キャッシュ
    gam_azimuth=gam.get_azimuth();//キャッシュ
    bool tl =(line.get_stat(0) || line.get_stat(1) || line.get_stat(2) ||line.get_stat(23) || line.get_stat(22))&&(line.get_stat(11) || line.get_stat(12) || line.get_stat(13) || line.get_stat(10) || line.get_stat(9));//縦ライン　前5つと後ろ5つが反応しているとき
    bool corner =(line.get_type()==2&&(getErr(line.get_pack(0),line.get_pack(1))<125));//実質的な角はこれに距離判定を入れる 3項ネストのところでやってる
    // if(SilentTime.read_milli()>(unsigned long)dash_border){dash();return;}//ダッシュ
    if(line_type==0){//ラインないとき
        SilentTime.reset();//ダッシュリセット
        mybuzzer.start(1500,999);//ぴぃー
        mymotor.run(lastdetect,200,0);//最後の向きに行く
        return;//関数強制終了
    }if(line_type==3){//3点ライン
        SilentTime.reset();//ダッシュリセット
        mymotor.run(0,200,0);//前行く
        return;//関数強制終了
    }if(ball_stat==false){//ボールない(見えない)
        mymotor.free();//動かない
        mybuzzer.start(250,999);//ブー
        SilentTime.reset();//ダッシュリセット
        return;//関数強制終了
    }
    mybuzzer.stop();//基本は鳴らさない
    lastdetect=line_azimuth;//最後の角度を保存
    applyXY(line_azimuth, line_x, line_y);//ラインのベクトル取得
    ball_ang = ball_azimuth + ball_cal;//ボール補正
    ball_x = ball_ang < 180 ? ball_x_calc : -ball_x_calc;//ボールのｘ 右左の01
    ball_y = isFront(ball_ang) ? ball_x_calc : -ball_x_calc;//y　これも上下で01
    calc_move_speed=tl ? move_speed>>1 : move_speed;//速度減算(1/2)
    line_x = corner ? line_x * line_powerup : line_x;//角補正
    line_y = corner ? line_y * line_powerup : line_y;//角補正
    ball_x = tl ? 0 : ball_x;//縦ラインではボールのxは無効
    line_x = !tl ? 0 : line_x;//縦ラインでなければラインのxは無効
    ball_y = !tl ? 0 : ball_y;//縦ラインでなければボールのyは無効
    move_x = (line_x + ball_x) * calc_move_speed;//移動x
    move_y = (line_y + ball_y) * calc_move_speed;//移動y
    if((tl&&isFront(ball_azimuth))||millis()<(unsigned int)last_exit_corner){//縦での脱出処理
        move_x = diff(line_x) * move_speed;//xを01で
        move_y = move_speed;//yは前に進むように
        if(tl&&isFront(ball_azimuth))last_exit_corner=millis()+50;//50ms抜けたあとも前に進むように
        else mybuzzer.start(1200,999);//デバッグ
    }
    // move_azimuth = norm360(myvector.get_azimuth(move_x, move_y));
    // move_azimuth= (corner && line_mag<7) ? (line_azimuth<180) ? 90 : 270 : move_azimuth;
    move_azimuth= (corner && line_mag<7) ? (line_azimuth<180) ? 90 : 270: norm360(myvector.get_azimuth(move_x, move_y));//移動角度計算　もし角ならラインの直角方向(並行線の方向)に向きを補正でなければxy合成
    if(line_mag > line_back_mag){//ラインに戻る　アウト防止
        mypixel.use_pixel(true);//一応ね　デバッグなので後で消す
        mypixel.closest(line_azimuth, 0, 255, 0, 1);//戻る意思表示(？？？)
        move_azimuth = line_azimuth;//もちろんラインの角度に戻る
        calc_move_speed = 220;//全力　ライントレースと向き同じやからいきなり変わることはないと思うけど
    }
    // move_power = myvector.get_magnitude(abs(move_x), abs(move_y));
    // move_power=getErr(0, ball_azimuth) < ball_move_border && (!tl) ? 0 : move_power;
    move_power= getErr(0, ball_azimuth) < ball_move_border && (!tl) ? 0 : myvector.get_magnitude(abs(move_x), abs(move_y));//移動力計算　ボールが中央に近ければ止まる出なければ合成

    if(move_power > move_border && (!tl || (tl && isFront(ball_azimuth)))){//動く　条件は動く力が小さすぎないことと縦ラインでではない||縦ラインでも前にボールがあること
        mymotor.run(move_azimuth - gam_azimuth, (int) move_power, 0);//移動実行
        if(MoveTime.read_milli() > (unsigned int) noise_border) SilentTime.reset();//ノイズ待ち時間過ぎたらダッシュの待ち時間をリセット
    }else{
        if(tl)SilentTime.reset();//縦ラインならダッシュ待ちリセット　縦ラインでの暴発防止　まあ前に動くけども
        MoveTime.reset();//動いてないのでリセット
        mymotor.free();//動かない
    }
    dhst=Dhs.read_milli();//処理速度取得
    //mybuzzer.start((int)scaleRange(0.0f, dash_border, 500.0f, 1500.0f, (float)SilentTime.read_milli()), 999);
}

int Defense::dhstget(void){return dhst;}

void Defense::reset(void){
    MoveTime.reset();
    SilentTime.reset();
}

void Defense::dash(void){//後でなんとかする　今は触らない
    MoveTime.reset();
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
        int dash_start = millis();
        while((millis() - dash_start) < 750){
            gam.read_azimuth();
            mymotor.run(0, 220, 0);
        }
        mymotor.run(line.get_eazimuth(),220,0);
        delay(500);
        dash_start = millis();
        ball.read();
        while((millis() - dash_start) < dash_time) {
            gam.read_azimuth();
            line.read();
            mymotor.run(ball.get_azimuth(), 220, 0);
            if(myswitch.check_toggle() == 0 || !isFront(ball.get_azimuth())) {
                SilentTime.reset();
                break;
            }
        }
        SilentTime.reset();
        mymotor.free();
        delay(10);
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