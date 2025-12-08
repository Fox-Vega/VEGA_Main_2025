// #include "Defense.h"
// #include "Input.h"
// #include "Output.h"
// #include "AIP.h"
// #include "Process.h"


// //staticのやつら
// // constexpr uint_fast8_t Defense::edge_list[6];//static変数用の定義

// ///@brief 0はラインの検出角度　1はその時の機体角度
// int Defense::lastdetect[2] = {0};

// int Defense::move_azimuth = 0;//static変数用の定義
// float Defense::move_power = 0.0f;//static変数用の定義
// double Defense::move_x = 0.0f;//static変数用の定義
// double Defense::move_y = 0.0f;//static変数用の定義
// int Defense::calc_move_speed = 0;//static変数用の定義
// int Defense::lastpower = 999;//static変数用の定義
// int Defense::last_x = 999;//static変数用の定義
// int Defense::last_y = 999;//static変数用の定義
// float Defense::rad = 0.0f;//static変数用の定義
// float Defense::ball_ang = 0.0f;//static変数用の定義
// float Defense::line_x = 0.0f;//static変数用の定義
// float Defense::line_y = 0.0f;//static変数用の定義
// float Defense::ball_x = 0.0f;//static変数用の定義
// float Defense::ball_y = 0.0f;//static変数用の定義
// Timer Defense::Dtimer;//static変数用の定義
// Timer Defense::SilentTime;//static変数用の定義
// Timer Defense::MoveTime;//static変数用の定義
// Timer Defense::ReturnTime;//static変数用の定義
// int Defense::calb = 0;//static変数用の定義
// bool Defense::tl = false;//static変数用の定義
// bool Defense::corner = false;//static変数用の定義
// bool Defense::frog1 = false;//static変数用の定義
// bool Defense::frog2 = false;//static変数用の定義
// Defense::RGBA Defense::background;//static変数用の定義
// Defense::RGBA Defense::P_line;//static変数用の定義
// Defense::RGBA Defense::P_ball;//static変数用の定義
// Defense::RGBA Defense::move_ang;//static変数用の定義
// Defense::RGBA Defense::dash_timer;//static変数用の定義
// // int Defense::ddddd = 0;//static変数用の定義
// int Defense::line_azimuth_cache = 0;//static変数用の定義
// int Defense::line_type_cache = 0;//static変数用の定義
// int Defense::line_x_cache = 0;//static変数用の定義
// int Defense::line_y_cache = 0;//static変数用の定義
// int Defense::ball_azimuth_cache = 0;//static変数用の定義
// bool Defense::ball_stat_cache = false;//static変数用の定義
// int Defense::gam_azimuth_cache = 0;//static変数用の定義




// void Defense::setup() {
//     reset();//初期化　その他は何にもない
//     //...あれれ？　意味ないじゃんけ
// }

// void Defense::defense_(int start_cord) {
//     // === 1. 入力データ取得・キャッシュ ===
//     if(start_cord != 0) {
//         int aaa = back_ang[start_cord-1];
//         mypixel.use_pixel(true);
//         mypixel.multi(0,15,255,255,255);
//         mypixel.closest(aaa,255,0,0,1);
//         mypixel.show();
//         mybuzzer.start(500,999);
//         delay(500);
//         while(!(line.get_type() == 2)) {
//             line.read();
//             ball.read();
//             gam.read_azimuth();
//             mymotor.run(aaa, 120, 0); //back_ang[start_cord-1]
//             if(myswitch.check_toggle() == 0) {
//                 return;
//             }
//         }
//         mymotor.run(0,200,0);
//         delay(100);
//         mymotor.free();
//         mybuzzer.stop();
//         mypixel.clears();
//         mypixel.shows();
//         mypixel.use_pixel(false);
//         return;
//     }else{
//         mypixel.clear();
//     }

//     // センサーデータを一度だけ読み取ってキャッシュ
//     line_azimuth_cache = line.get_azimuth();
//     line_type_cache = line.get_type();
//     line_x_cache = line.get_x();
//     line_y_cache = line.get_y();
//     ball_azimuth_cache = ball.get_azimuth();
//     ball_stat_cache = ball.get_stat();
//     gam_azimuth_cache = gam.get_azimuth();
//     calb = 0 - gam_azimuth_cache;
    
//     // 縦ライン判定
//     tl = false;
//     frog1 = line.get_stat(0) || line.get_stat(1) || line.get_stat(2) ||
//                 line.get_stat(23) || line.get_stat(22);
//     frog2 = line.get_stat(11) || line.get_stat(12) || line.get_stat(13) ||
//                 line.get_stat(10) || line.get_stat(9);
//     if(frog1 && frog2) {
//         tl = true;
//     }

//     corner =(line.get_type()==2&&(getErr(line.get_pack(0),line.get_pack(1))<110));

//     // === 2. ダッシュ判定（最優先） ===
//     if((SilentTime.read_milli() > dash_border && USE_DASH == true)) {
//         dash();
//         return;
//     }

//     // === 3. フラグ決定 ===
//     frog = FROG::NONE;

//     if(line_type_cache == 0) {
//         // ライン無し
//         frog = FROG::NO_LINE;
//     } else if(line_type_cache == 3) {
//         // 角ライン（特殊処理）
//         frog = FROG::BAD_LINE;
//     } else if(!ball_stat_cache) {
//         // ボール無し
//         frog = FROG::NO_BALL;
//     } else {
//         // 通常モード
//         frog = FROG::NORMAL;
//     }

//     // === 4. フラグに応じた動作実行 ===
//     switch(frog) {
//         case FROG::NORMAL:
//             normal();
//             break;
//         case FROG::NO_LINE:
//             noline();
//             break;
//         case FROG::BAD_LINE:
//             // 3角ライン処理
//             // mybuzzer.start(1500, 999);
//             // mymotor.run(0,200,0);
//             // delay(vertical_return);
//             // mybuzzer.stop();
//             break;
//         case FROG::NO_BALL:
//             noball();
//             break;
//         case FROG::STOP:
//             mymotor.free();
//             break;
//         case FROG::DASH:
//             // ダッシュ処理は上で実行済み
//             break;
//         default:
//             mymotor.free();
//             break;
//     }

//     // === 5. UI更新 ===
//     //applyUI(static_cast<int>(frog));

//     // === 6. タイマー処理 ===
//     // int ddddd = Dtimer.read_milli();
//     // Serial.println(ddddd);
//     updateTimers();
// }

// void Defense::dash() {
//     frog = FROG::DASH;
//     float TL = 20.0;
//     float TLM = 60.0;

//     if(SilentTime.read_milli() < dash_border * 1.2) {
//         if(myswitch.check_toggle() == 0) {
//             SilentTime.reset();
//             return;
//         }

//         //mypixel.multi(0, 15, 255, 50, 50);
//         mypixel.show();
//         SilentTime.reset();
//         // アタック
//         while(SilentTime.read_milli() < dash_time) {
//             gam.read_azimuth();
//             ball.read();
//             line.read();

//             mymotor.run(0, 220, 0);
//             if(myswitch.check_toggle() == 0||!isFront(ball.get_azimuth())) {
//                 SilentTime.reset();
//                 break;
//             }
//         }
//         SilentTime.reset();
//         mymotor.free();

//         delay(10);//matuuuuuuuuuuuuu

//         // // 後退
//         // while(SilentTime.read_milli() < 300) {
//         //     gam.read_azimuth();
//         //     mymotor.run(180, 200, 0);
//         //     if(myswitch.check_toggle() == 0) {
//         //         SilentTime.reset();
//         //         break;+
//         //     }
//         // }
//         // SilentTime.reset();

//         // 復帰
//         int mm = 180;
//         int mt = 75;
//         while(1) {
//             gam.read_azimuth();
//             ball.read();
//             line.read();
//             if(line.get_type() == 1) {
//                 if((line.get_azimuth() < 180 - TL && line.get_azimuth() > TL) ||
//                     (line.get_azimuth() > 180 + TL && line.get_azimuth() < 360 - TL)) {
//                     if(line.get_azimuth() < 180 - TL && line.get_azimuth() > TL) {
//                         mm = 180 - TLM;
//                     } else {
//                         mm = 180 + TLM;
//                     }
//                 }
//                 break;
//             }
//             mymotor.run(mm, static_cast<int>(mt), 0);
//             if(mt < 120) mt += 1;
//             if(myswitch.check_toggle() == 0) {
//                 SilentTime.reset();
//                 break;
//             }
//         }
//         mymotor.free();
//         delay(100);
//     } else {
//         SilentTime.reset();
//     }
// }


// void Defense::normal() {
//     mybuzzer.stop();
    
//     // キャッシュを使用（メソッド呼び出し削減）
//     lastdetect[0] = line_azimuth_cache;
//     lastdetect[1] = gam_azimuth_cache;


//     rad = radians(line_azimuth_cache);
//     line_x = sin(rad);
//     line_y = cos(rad);

//     // ball方向ベクトル計算
//     ball_ang = ball_azimuth_cache + ball_cal;
//     ball_y = (ball_ang < 90 || ball_ang > 270) ? 1 : -1;
//     ball_x = (ball_ang < 180) ? 1 : -1;

//     //多分消してはいけないーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
//     // 速度減算（軽量化）
//     calc_move_speed = (line_x_cache > 3 || tl) ? move_speed / 2 : move_speed;
//     //calc_move_speed=move_speed;//上直したらこれ消す

//     // X軸移動量計算
//     if(tl) ball_x = 0;
//     if(corner) line_x *= 1.5;
//     //これも消してはいけないーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
//     //if(corner) line_y *= 1.5;//もしかするとこれ消して角改善
    
//     move_x = (line_x + ball_x) * calc_move_speed;
//     if(tl && abs(line_x) < 2) move_x = 0;//ただの改善

//     // Y軸移動量計算
//     if(corner) {
//         line_y *= 1.5;
//     } else {
//         if(!tl && abs(line_x_cache) < 2) ball_y = 0;//消すかもね　やりますね
        
//         if(tl) line_y = 0;
//         line_y *= 1.4;//倍率基本は1
//     }
// //ここだよーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
//     move_y = (line_y + ball_y) * calc_move_speed;
//     // if((!tl) && abs(line_y_cache) < 1) move_y /= 2;//並行ラインのy減算　これちっちゃくしたらまだ増しになるのかもしれない

//     // 最終移動ベクトル計算
//     move_azimuth = myvector.get_azimuth(move_x, move_y);
//     move_power = myvector.get_magnitude(abs(move_x), abs(move_y));

//     // 停止判定
//     if(getErr(0, ball_azimuth_cache) < ball_move_border && !tl) {
//         move_power = 0;
//     }

//     // モーター制御
//     if (move_power > move_border && (!tl || (tl&&isFront(ball_azimuth_cache)))) {
//         mymotor.run(move_azimuth, (int)move_power, 0);  // 軽量キャスト
//         if(MoveTime.read_milli() > 300) {
//             SilentTime.reset();
//         }
//     } else {
//         if(tl){
//             SilentTime.reset();
//             mymotor.run(0,100,0);
//         }
//         mymotor.free();
//         frog = FROG::STOP;
//         MoveTime.reset();
//         if(!tl)mybuzzer.start((int)scaleRange(0.0f, dash_border, 500.0f, 1500.0f, (float)SilentTime.read_milli()), 999);
//     }

//     // 保存
//     lastpower = (int)move_power;
//     last_x = (int)line_x;
//     last_y = (int)line_y;
// }



// void Defense::reset() {
//     resetUI();
//     Dtimer.reset();
//     SilentTime.reset();
//     MoveTime.reset();
//     ReturnTime.reset();
// }


// void Defense::resetUI() {
// }

// void Defense::noline() {
//     mybuzzer.start(2000,999);
//     mypixel.multi(0,15,255,255,255);
//     mypixel.shows();
//     if(ReturnTime.read_milli()>2000){
//         mymotor.stabilization(0);
//         mymotor.run(norm360(lastdetect[0]+(lastdetect[1]-gam.get_azimuth())), 230,0);
//         mymotor.stabilization(1);
//     }
//     else
//         mymotor.run(norm360(lastdetect[0]), 230, 0);
// }

// void Defense::noball() {
//     mypixel.use_pixel(true);
//     mypixel.multi(0,15,255,0,0);
//     mypixel.shows();
//     mybuzzer.start(500, 999);
//     mymotor.free();
// }


// void Defense::updateTimers() {
//     if(static_cast<int>(frog) != 4 && MoveTime.read_milli() > noise_border) {
//         SilentTime.reset();
//     }

//     if(line.get_type() != 0) {
//         ReturnTime.reset();
//     }

//     Dtimer.reset();
// }

// void Defense::applyUI(int mode) {
// }
