#include "Defense.h"
#include "Input.h"
#include "Output.h"
#include "AIP.h"
//#include "okomeonigiri.h"

#define printf_s(fmt, ...) ({ char buf[256]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
//buf size:128  short
#define printf_s_short(fmt, ...) ({ char buf[128]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
//buf size:512  long
#define printf_s_long(fmt, ...) ({ char buf[512]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
//buf size:custom  // Use a custom buffer size defined by the user
#define printf_s_custom(bufsize,fmt, ...) ({ char buf[bufsize]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })
//buf size:SIZE_MAX  // Use the maximum size of a buffer, which is defined by SIZE_MAX
#define printf_s_max(fmt, ...) ({ char buf[SIZE_MAX]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })

//A for loop that allows you to easily use a for loop with just a single number.
#define floop(n) for(size_t i = 0; i < n; ++i)
#define floop_R(n) for(size_t i = n; i> cn; --i)
//A floop that allows you to use any variable name you like as the loop counter.
#define floop_id(var, n) for(int var = 0; var < (n); ++var)
#define floop_id_R(var, n) for(int var = (n); var > 0; --var)

data Dball;
data Dline;

void Defense::setup(void){
    mybuzzer.start(1000, 200);
    general.setup();
}

void Defense::defense_(void){
    setup();
    while(true){
        get_vector();
        if(Dline.detect){
            if(line.get_type()==1){
                p1();
            }
        }
        else{
            while(1){
                GoBackLine();
                get_vector();
                if(Dline.detect)break;
            }
        };
    }
}

void Defense::p1(void){
    int move_x=0;
    int move_y=0;
}

void Defense::GoBackLine(){
    int last_line_time=0;
    for(byte i=0; i<50;i++){
        if(line_history[0][50-i]==1){
            last_line_time=50-1;
            break;
        }
    }
    mymotor.run(line_history[1][last_line_time],120,0);
}

void Defense::get_vector(void)
{
    // ライン情報の取得
    line.read();
    Dline.azimuth = line.get_azimuth();
    Dline.dist = line.get_magnitude() * ball_max / 22;
    Dline.detect = (Dline.dist == 999) ? false : true;
    Dline.x = Dline.dist * cos(radians(Dline.azimuth));
    Dline.y = Dline.dist * sin(radians(Dline.azimuth));

    // 履歴保存（history 0:detect 1:azimuth 2:dist）
    if (line_history_index >= 50) {
        // 古い順に削除（前に詰める）
        for (int i = 1; i < 50; i++) {
            line_history[0][i - 1] = line_history[0][i];
            line_history[1][i - 1] = line_history[1][i];
            line_history[2][i - 1] = line_history[2][i];
        }
        line_history_index = 49;
    }
    line_history[0][line_history_index] = Dline.detect;
    line_history[1][line_history_index] = Dline.azimuth;
    line_history[2][line_history_index] = Dline.dist;
    line_history_index++;

    // ボール情報の取得
    ball.read();
    Dball.azimuth = ball.get_azimuth();
    Dball.dist = (int)ball.get_magnitude();
    Dball.detect = ((int)ball.get_magnitude() > 0) ? true : false;
    myvector.get_cord(Dball.azimuth, Dball.dist);
    Dball.x = myvector.get_x();
    Dball.y = myvector.get_y();

    // 履歴保存（history 0:detect 1:azimuth 2:dist）
    if (ball_history_index >= 50) {
    // 古い順に削除（前に詰める）
    for (int i = 1; i < 50; i++) {
        ball_history[0][i - 1] = ball_history[0][i];
        ball_history[1][i - 1] = ball_history[1][i];
        ball_history[2][i - 1] = ball_history[2][i];
    }
    ball_history_index = 49;
}
ball_history[0][ball_history_index] = Dball.detect;
ball_history[1][ball_history_index] = Dball.azimuth;
ball_history[2][ball_history_index] = Dball.dist;
ball_history_index++;
}

// void Defense::MyUI(int mode){
//     mode=0;
//     int tact=0;
//     int toggle=0;
//     while (true) {
//         tact = myswitch.check_tact();
//         toggle = myswitch.check_toggle();
//         if(toggle==1)
//         {
//             mybuzzer.start(500, 200);
//             while(toggle==1)
//             {
//                 if(tact != 0) // タクトスイッチが押されたら
//                 {
//                     mybuzzer.start(500, 200);
//                     tact = myswitch.check_tact();
//                     if(tact == 1) // タクトスイッチが押されたら
//                     {
//                         mode++;
//                         if(mode > 2) mode = 0; // モードを0~2の範囲に制限
//                     }
//                     else if(tact == 2) // タクトスイッチが押されたら
//                     {
//                         mode--;
//                         if(mode < 0) mode = 2; // モードを0~2の範囲に制限
//                     }
//                 }
//                 printf_s("select mode...\n");
//                 switch(mode)
//                 {
//                     case 0:
//                         printf_s("ball <<  line  >>  pixel\n");
//                         break;
//                     case 1:
//                         printf_s("line <<  ball  >>  pixel\n");
//                         break;
//                     case 2:
//                         printf_s("line <<  pixel  >>  ball\n");
//                         break;
//                 }
//                 for(uint8_t i = 0; i < 10; i++) {
//                     printf_s("\n");
//                 }
//                 printf_s("left  switch  left   debug\n");
//             }
//             }
//             else
//             {
//                 switch(mode){//0 ball 1 line 2 pixel
//                     case 0:
//                         get_vector_Ball();
//                         printf_s("Ball Vector: Theta: %d\nDist: %d\nX: %.2f\nY: %.2f\nDetect: %s\n"
//                             ,Dball.azimuth, Dball.dist, Dball.x, Dball.y, Dball.detect ? "true" : "false");
//                         break;
//                 }
//             }
//         }
//     }

// void MyUI_clear() {
//     for (uint8_t i = 0; i < 50; i++) {
//         printf_s("\n");
//     }
// }