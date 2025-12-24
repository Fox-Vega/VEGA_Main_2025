#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
#include "Process.h"

inline void ToggleReturn() {if(myswitch.check_toggle() == 0) return;}
#define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })//Serial.printのprintfフォーマット版

constexpr int ball_x_calc=1;

// Timer変数の実体定義
Timer SilentTime,MoveTime;

// 復帰方向リストの定義
constexpr int back_ang[4] = {180, 180, 225, 135};

// グローバル変数の定義（Defense.cppでのみ定義）
static int lastdetect = 0;

//-----調整用定数-----//

///　@brief ダッシュ待ち時間
constexpr unsigned long  dash_border = 2000;
/// @brief 基本移動速度
constexpr int move_speed = 200;
/// @brief 最小移動速度
const int move_border = 30;
/// @brief ボール補正角度
constexpr int ball_cal =0;
//// @brief ダッシュ時間
constexpr unsigned long dash_time = 200;
/// @brief ボール移動境界(±角度)
constexpr float ball_move_border = 2;
/// @brief ノイズ除去
constexpr unsigned long noise_border = 200;
/// @brief ライン強化
constexpr int line_back_mag=9;
/// @brief ライン強化2 ms
constexpr int line_least_times=100;

//-----調整できない定数-----//

//処理速度
int dhst;
Timer Dhs;

/// @brief 初期化
void Defense::setup(void){reset();}
void Defense::defense_(int start_cord){
    Dhs.reset();
    mypixel.use_pixel(true);//ピクセル使用
    if(start_cord != 0) {//開始座標が渡されたとき　{割り込み処理}
        while(!(line.get_type() == 2)) {//並行ラインまで待つ
            line.read();//ライン読み
            gam.read_azimuth();//これは多分姿勢制御に必要と見て読んでる
            mymotor.run(back_ang[start_cord-1], 180, 0);//決められた角度に進むだけ　基本後ろか左後ろか右後ろ　特に特別な処理はない
            ToggleReturn();
        }
        mymotor.run(0,200,0);//少し前に進んでおく　前にいておけばなんとかる
        delay(100);//進む時間
        return;//関数を強制終了してその後はmainで座標がリセットされつ　つまりここが呼び出されることは一度以降ない
    }
    const int line_azimuth = line.get_azimuth();//キャッシュ
    const int line_type = line.get_type();//キャッシュ
    const int line_mag = line.get_magnitude();//キャッシュ
    const int ball_azimuth = ball.get_azimuth();//キャッシュ
    const int ball_stat = ball.get_stat();//キャッシュ
    const int ball_intensity = ball.get_intensity();//キャッシュ
    const int gam_azimuth = gam.get_azimuth();//キャッシュ
    float move_power,move_x,move_y,ball_x,ball_y,line_x,line_y= 0.0f;//変数定義
    int calc_move_speed,ball_ang,move_azimuth = 0;//変数定義
    // 縦ライン判定：前後のセンサ群が反応しているか（前7つ + 後ろ7つ）
    // bool tl = (line.get_stat(0) || line.get_stat(1) || line.get_stat(2) || line.get_stat(3) || line.get_stat(23) || line.get_stat(22) || line.get_stat(21))&& (line.get_stat(9) || line.get_stat(10) || line.get_stat(11) || line.get_stat(12) || line.get_stat(13) || line.get_stat(14) || line.get_stat(15));
    bool tl =
    (line.get_stat(23) || line.get_stat(0) || line.get_stat(1) || line.get_stat(2) || line.get_stat(3)) &&
    (line.get_stat(10) || line.get_stat(11) || line.get_stat(12) || line.get_stat(13) || line.get_stat(14));
    bool corner = line_type==2&&isDiagonalAngle(norm360(line.get_azimuth()+gam.get_azimuth()));
    bool edge = isInSide30(norm360(line_azimuth + gam_azimuth));//辺検知かつ前方30度以内
    if(SilentTime.read_milli()>(unsigned long)dash_border||ball_intensity>32){dash(tl);return;}//ダッシュ
    if(line_type==0){//ラインないとき
        mypixel.closest(lastdetect, 255, 0, 0, 1);//赤表示
        mypixel.shows();
        SilentTime.reset();//ダッシュリセット
        int line_detect_time=0;
        while(line.get_type()==0){//ライン見えるまでループ
            mypixel.closest(lastdetect, 255, 0, 0, 1);//赤表示
            mypixel.shows();
            line.read();//ライン読み
            gam.read_azimuth();//姿勢制御用
            mymotor.run(lastdetect,200,0);//最後の向きに行く
            if(myswitch.check_toggle() == 0) return;//トグルで終了させるための処理
            if(!line.get_type()==0)line_detect_time=millis()+line_least_times;//ライン見えたら時間セット
            else if(line_detect_time!=0 && (int)millis()>line_detect_time)break;//ライン見えてから一定時間経ったら抜ける
        }
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
    ball_x = ball_ang < 180 ? ball_x_calc : -1*ball_x_calc;//ボールのｘ 右左の01
    ball_y = isFront(ball_ang) ? ball_x_calc : -1*ball_x_calc;//y　これも上下で01

    calc_move_speed = corner ? move_speed>>1 : move_speed;//速度減算(1/2)

    line_x = !tl ? 0 : line_x;//並行ラインならラインのxは無効
    line_y = tl ? 0 : line_y;//縦ラインならラインのyは無効
    line_y = line_mag<5 ? 0:line_y;

    ball_x = tl ? 0 : ball_x;//縦ラインではボールのxは無効
    ball_y = !tl ? 0 : ball_y;//並行ラインならボールのyは無効

    move_x = (line_x + ball_x) * calc_move_speed;//移動x
    move_y = (line_y + ball_y) * calc_move_speed;//移動y

    if(corner){//角処理
        applyXY(line_azimuth, line_x, line_y);//ライン直角方向ベクトル取得
        ball_x = ball_ang < 180 ? ball_x_calc : -1*ball_x_calc;//ボールのｘ 右左の01
        move_x = diff_signs(ball_x,line_x) ? 0 : diff(ball_x)*move_speed;//x成分打ち消し
        move_y = 0;
    }

    if(((tl&&isFront(ball_azimuth))&&!corner)||(edge&&!corner)){//縦での脱出処理
        applyXY(line_azimuth, line_x, line_y);//ライン直角方向ベクトル取得
        move_x = line_mag<5 ? 0 : diff(line_x) * move_speed;//xを01で
        move_y = move_speed;//yは前に進むように
    }

    move_azimuth=norm360(myvector.get_azimuth(move_x, move_y));//移動角度計算　もし角ならラインの直角方向(並行線の方向)に向きを補正でなければxy合成
    if(line_mag > line_back_mag){//ラインに戻る　アウト防止
        mypixel.use_pixel(true);//一応ね　デバッグなので後で消す
        //mypixel.closest(line_azimuth, 0, 255, 0, 1);//戻る意思表示(？？？)
        move_azimuth = line_azimuth;//もちろんラインの角度に戻る
        calc_move_speed = 220;//全力　ライントレースと向き同じやからいきなり変わることはないと思うけど
    }

    move_power= ((getErr(0, ball_azimuth) < ball_move_border) && (line_mag<6) && (!tl)) ? 0 : myvector.get_magnitude(abs(move_x), abs(move_y));//移動力計算　ボールが中央に近ければ止まる出なければ合成
    // move_power= (corner && line.get_magnitude() < 4) ? 0 : move_power;//角なら止まる
    if((move_power > move_border) && (!tl || (tl && isFront(ball_azimuth)))){//動く　条件は動く力が小さすぎないことと縦ラインでではない||縦ラインでも前にボールがあること
        mymotor.run(move_azimuth, (int) move_power,0);//移動実行
        if((MoveTime.read_milli() > (unsigned int) noise_border)&&!corner) SilentTime.reset();//ノイズ待ち時間過ぎたらダッシュの待ち時間をリセット
    }else{
        if(tl && !corner){SilentTime.reset();};//縦ラインならダッシュ待ちリセット　縦ラインでの暴発防止　まあ前に動くけども
        MoveTime.reset();//動いてないのでリセット
        mymotor.free();//動かない
    }
    // mypixel.closest(line_azimuth, 255, 255, 0, 1);//ボール方向表示　デバッグ
    mypixel.multi(0,scaleRange(0.0f, dash_border, 0.0f, 15.0f, (float)SilentTime.read_milli()), 0, 255, 0);//ダッシュ待ち時間表示　デバッグ
    dhst=Dhs.read_milli();//処理速度取得
}

int Defense::dhstget(void){return dhst;}

void Defense::reset(void){
    MoveTime.reset();
    SilentTime.reset();
}

void Defense::dash(bool tl){//後でなんとかする　今は触らない
    MoveTime.reset();
    if((SilentTime.read_milli() < dash_border * 1.2)) {
        if(myswitch.check_toggle() == 0) {
            SilentTime.reset();
            return;
        }
        SilentTime.reset();
        mypixel.multi(0, 15, 255, 0, 0);//ダッシュ表示　デバッグ
        mypixel.shows();
        while(SilentTime.read_milli() < dash_time) {
            gam.read_azimuth();
            line.read();
            ball.read();
            mymotor.run(0, 220, 0);
            if(myswitch.check_toggle() == 0 || !isFront(ball.get_azimuth())) {
                SilentTime.reset();
                break;
            }
        }
        SilentTime.reset();
        mymotor.free();
        delay(50);
        lastdetect = 180;
        mypixel.multi(0, 15, 0, 255, 0);//ダッシュ終了表示　デバッグ
        mypixel.shows();
        while(1){
            line.read();
            gam.read_azimuth();
            if(line.get_type() == 2) {
                SilentTime.reset();
                break;
            }
            mymotor.run(180, 150, 0);
            if(myswitch.check_toggle() == 0) {
                SilentTime.reset();
                break;
            }
        }
        SilentTime.reset();
        mypixel.multi(0, 15, 0, 0, 255);//ダッシュ表示　デバッグ
        mypixel.shows();
        while(SilentTime.read_milli() < 500 && line.get_magnitude()>4){
            line.read();
            gam.read_azimuth();
            mymotor.run(line.get_azimuth(), 180, 0);
            if(myswitch.check_toggle() == 0) {
                SilentTime.reset();
                break;
            }
        }
        mypixel.clears();
    } else {
        SilentTime.reset();
    }
}