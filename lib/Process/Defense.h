#pragma once
#include <Arduino.h>
#include <Timer.h>

/*
 * ██████╗ ███████╗███████╗███████╗███╗   ██╗███████╗███████╗
 * ██╔══██╗██╔════╝██╔════╝██╔════╝████╗  ██║██╔════╝██╔════╝
 * ██║  ██║█████╗  █████╗  █████╗  ██╔██╗ ██║███████╗█████╗
 * ██║  ██║██╔══╝  ██╔══╝  ██╔══╝  ██║╚██╗██║╚════██║██╔══╝
 * ██████╔╝███████╗██║     ███████╗██║ ╚████║███████║███████╗
 * ╚═════╝ ╚══════╝╚═╝     ╚══════╝╚═╝  ╚═══╝╚══════╝╚══════╝
*/

/**
 * @file Defense.h
 * @brief Defenseのヘッダファイル
 * @author okomeonigiri
 * @note 変数を外でstaticしているのは早いから
 * @note Doxygenコメント書いてもコンパイルで消えるからね
*/


//-----キャッシュたち-----//
/**
 * @note こっちのほうが早いの
 * @defgroup キャッシュ変数
 * @brief 高速化のためのキャッシュ
 */

/**
 * @brief line.get_azimuth()のキャッシュ
 * @ingroup キャッシュ変数
 */
static int line_azimuth;

/**
 * @brief line.get_type()のキャッシュ
 * @ingroup キャッシュ変数
 */
static int line_type;

/**
 * @brief ball.get_azimuth()のキャッシュ
 * @ingroup キャッシュ変数
 */
static int ball_azimuth;

/**
 * @brief ball.get_stat()のキャッシュ
 * @ingroup キャッシュ変数
 */
static int ball_stat;

/**
 * @brief gam.get_azimuth()のキャッシュ
 * @ingroup キャッシュ変数
 */
static int gam_azimuth;

/*
 *  staticは静的なので早いと
 *  クラス内に定義しないほうが早いのもある
*/
//-----普通の変数たち-----//

/// @brief  縦ライン
static bool tl;

/// @brief 角
static bool corner; //コーナーライン判定

/// @brief 動く方向
static int move_azimuth;
/// @brief 動くパワー
static float move_power;
/// @brief 動くX
static float move_x;
/// @brief 動くY
static float move_y;
/**
 * @brief 減算とかしたあとの速度。
 * \n 減算は角とか縦とかでアウト防止として行ったりする
 */
static int calc_move_speed;
/// @brief ラインの計算用X
static int line_x;
/// @brief ラインの計算用Y
static int line_y;
/// @brief ボールの計算用角度
static int ball_ang;
/// @brief ボールの計算用X
static int ball_x;
/// @brief ボールの計算用Y
static int ball_y;
/// @brief ラジアン
static float rad;
/// @brief 最後のラインの検出角度
static int lastdetect;

/// @brief 復帰の方向リスト
static constexpr int back_ang[4]={180,180,225,135};

// /// @brief フラグ　再利用可能
// static bool frog1;
// /// @brief フラグ2　再利用可能
// static bool frog2;



/**
 * @note 疑似タイマーみたいなものの値定義場所
 * @attention アタックと同じや
 * 普通にクラス使ったほうが良くないか　作った意味よ
 */


/// @brief ダッシュ待ち時間カウント用
static int LastSilent;

/// @brief 移動時間カウント用
static int LastMove;

/// @brief ライン復帰時間カウント用
static int LastReturn;



class Defense{
public:
    /// @brief　初期化
    void setup(void);
    /// @brief Defenseのメイン処理
    /// @param start_cord 開始座標
    void defense_(int start_cord);
    /// @brief タイマーなどの状態のリセット
    void reset(void);
private:
    /**
     * @brief ダッシュ処理
     * ダッシュの条件揃ったら呼び出して
     * だが中でwhile回してるからってところと
     * 一回で時間のかかる割り込み処理して抜けるってところ
     * 中で一応トグルで終了はつけてある
     * @return んなものない　voidやがな
    */
    void dash(void);

    //-----調整用定数-----//
    static constexpr int dash_border = 15000; //ダッシュ待ち時間
    static constexpr int move_speed = 200; //基本移動速度
    static constexpr int move_border = 30; //最小移動速度
    static constexpr int ball_cal =-10; //ボール補正角度
    /// @brief フラグ
    enum class FROG : int {
        //何もなし
        NONE = 0,
        //通常動作
        NORMAL= 1,
        //縦ライン
        VERTICAL_LINE = 2,
        //ライン無し
        NO_LINE = 3,
        //ボール無し
        NO_BALL = 4,
        //ダッシュ中
        DASH = 5,
        //ダッシュ終わり
        DASH_END = 7,
        //三点ラインとかそことか
        BAD_LINE = 6,
    }frog;

    /// @brief 360度の範囲に正規化する
    /// @param a 正規化する角度　単位：度
    /// @return ない
    inline void norm360P(int &a) {
        a %= 360;
        if(a < 0) a += 360;
    }

    /// @brief 360度の範囲に正規化する
    /// @param a 正規化する角度　単位：度
    /// @return 正規化された角度
    inline int norm360(int &a) {
        a %= 360;
        if(a < 0) a += 360;
        return a;
    }

    /// @brief 前か後ろか判定
    /// @param angle 判定する角度
    /// @return 前であればtrue、後ろであればfalse
    inline bool isFront(int angle) {
        angle = norm360(angle);
        return (angle <=90||angle>=270);
    }


    /// @brief aとbの角度上での差分を返す
    /// @param a 値1
    /// @param b 値2
    /// @return 小さい方の差分の絶対値
    inline static int getErr( int a, int b) { int d = abs((a - b) % 360); return (d > 180) ? (360 - d) : d; }


};
