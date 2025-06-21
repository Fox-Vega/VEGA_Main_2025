#include "Defense.h"
#include "Input.h"
#include "Output.h"
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
//A floop that allows you to use any variable name you like as the loop counter.
#define floop_id(var, n) for(int var = 0; var < (n); ++var)

extern LINE line;

data Dball;
data Dline;

void Defense::defense_(void){
    mybuzzer.start(1000, 200);
    Serial.println("Defense Process Start");
    general.setup();
    while(true){
        if(line.read()!=true){
            get_vector();
            int go_ang = 999; //目標角度
            int line_mod =
        }
        else Dline_not();
    }
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


void Defense::get_vector_Line(void)
{
    Dline.azimuth = line.get_azimuth();
    Dline.dist = line.get_magnitude()*900/22;
    Dline.detect = (line.read()) ? false : true; // ラインが検出されていない場合はfalse
    Dline.x = Dline.dist * cos(radians(Dline.azimuth));
    Dline.y = Dline.dist * sin(radians(Dline.azimuth));
}

void Defense::get_vector_Ball(void)
{
    Dball.azimuth = ball.get_azimuth();
    Dball.dist = (int)ball.get_magnitude();
    Dball.detect = ((int)ball.get_magnitude() > 0) ? true : false;
    myvector.get_cord(Dball.azimuth, Dball.dist);
    Dball.x = myvector.get_x();
    Dball.y = myvector.get_y();
}
