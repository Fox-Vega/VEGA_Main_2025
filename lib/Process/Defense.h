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
 * @note Doxygenとかコメント多い？　コンパイルするんだからいいだろ
 * @todo コメ地獄　//まあまあ
 * @todo 書き換え完成　//もうすぐ
 * @todo Old消したい　//上に依存
 * @todo　並行ライン爆速目指して　//実際に試したいところ　大体処理速度かな
*/

/*
 *  staticは静的なので早いと
 *  クラス内に定義しないほうが早いのもある
*/
//-----普通の変数たち-----//
// 変数の実体はDefense.cppで定義済み

/// @brief 復帰の方向リスト
extern const int back_ang[4];

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
extern Timer SilentTime;

/// @brief 移動時間カウント用
extern Timer MoveTime;

/// @brief ライン復帰時間カウント用
extern int LastReturn;



class Defense{
public:
    /// @brief　初期化
    void setup(void);
    /// @brief Defenseのメイン処理
    /// @param start_cord 開始座標
    /// @return ない うん
    /// @note 遅いのなおしたい
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

    ///　@brief ダッシュ待ち時間
    const int dash_border = 15000;
    /// @brief 基本移動速度
    const int move_speed = 200;
    /// @brief 最小移動速度
    const int move_border = 30;
    /// @brief ボール補正角度
    const int ball_cal =-10;
    //// @brief ダッシュ時間
    const unsigned int dash_time = 2000;
    /// @brief ボール移動境界(±角度)
    const float ball_move_border = 7.5;
    /// @brief ノイズ除去
    const int noise_border = 300;
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
    /// @note 基本こっちで行きたいなぁ
    inline void norm360P(int &a) {
        a %= 360;
        if(a < 0) a += 360;
    }

    /// @brief 360度の範囲に正規化する
    /// @param a 正規化する角度　単位：度
    /// @return 正規化された角度
    /// @note 条件式で保険としてかけるやつ　リターンする方
    inline int norm360(int a) {
        a %= 360;
        if(a < 0) a += 360;
        return a;
    }

    /// @brief 前か後ろか判定
    /// @param angle 判定する角度
    /// @return 前であればtrue、後ろであればfalse
    /// @note inlineやから条件式書くのと変わりませんと　見やすいぐらいか
    inline bool isFront(int angle) {
        norm360P(angle);
        return (angle <=90||angle>=270);
    }


    /// @brief aとbの角度上での差分を返す
    /// @param a 値1
    /// @param b 値2
    /// @return 小さい方の差分の絶対値
    /// @note inlineやから条件式書くのと変わりませんと　見やすいぐらいか
    inline static int getErr( int a, int b) { int d = abs((a - b) % 360); return (d > 180) ? (360 - d) : d; }


};
