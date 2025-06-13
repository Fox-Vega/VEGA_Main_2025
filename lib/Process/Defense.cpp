// #include "Defense.h"

// // extern LINE line;

// data Dball;
// data Dline;

// //printf
// #define printf_s SERIAL_PRINTF
// #define SERIAL_PRINTF(fmt, ...) ({ char buf[512]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print(buf); })

// void Defense::setup(void)
// {
// }

// void Defense::defense_(void)
// {
//     Serial.println("Defense Process Start");
//     general.setup();
//     while(true)
//     {
//         if(myswitch.check_toggle()==0) // タクトスイッチが押されたら
//         {
//             mybuzzer.start(1000, 200);
//             MyUI(1);
//         }
//         }
//     }


// void Defense::MyUI(int mode){
//     int mode=0;
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
//                             ,Dball.theata, Dball.dist, Dball.x, Dball.y, Dball.detect ? "true" : "false");
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


// void Defense::get_vector_Line(void)
// {
//     Dline.theata = line.get_azimuth();
//     Dline.dist = line.get_magnitude();
//     Dline.detect = (Dline.dist) ? false : true; // ラインが検出されていない場合はfalse
//     Dline.x = Dline.dist * cos(radians(Dline.theata));
//     Dline.y = Dline.dist * sin(radians(Dline.theata));
// }

// void Defense::get_vector_Ball(void)
// {
//     Dball.theata = ball.get_azimuth();
//     Dball.dist = (int)ball.get_magnitude();
//     Dball.detect = ((int)ball.get_magnitude() > 0) ? true : false;
//     myvector.get_cord(Dball.theata, Dball.dist);
//     Dball.x = myvector.get_x();
//     Dball.y = myvector.get_y();
// }