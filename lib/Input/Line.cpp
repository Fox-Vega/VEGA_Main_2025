#include <Line.h>
#include <Input.h>
#include <Output.h>
#include "okomeonigiri.h"
//TODO消した　あいつはいいやつだったよ…（？）

void LINE::setup(void) {
    pinMode(selectA, OUTPUT);
    pinMode(selectB, OUTPUT);
    pinMode(selectC, OUTPUT);
    pinMode(readPin1, INPUT);
    pinMode(readPin2, INPUT);
    pinMode(readPin3, INPUT);
}

void LINE::serial_print(void){
    printf_s("line          %d\n",read());
    //printf_s("%d",get_azimuth());
    // for(size_t i = 0; i < NUMLines; i++) {
    //     if(line_status[i]) {
    //         printf_s("Line %d: %d \n", i, line_memory[i]);
    //     } else {
    //         printf_s("Line %d: No Line \n", i);
    //     }
    // }
}

int LINE::get_azimuth(void) {
    read();
    if(read() == false) { // ラインが検出されていない場合
        return 999; // エラー値を返す
    }
    for(size_t i = 0; i < 4; i++) line_detect[i] = 999; // ライン検出配列の初期化
    size_t linecount = 0;
    int linemem[NUMLines] = {0};
    int linemem2[NUMLines] = {0};
    for(size_t i = 0; i < NUMLines; i++) if(line_status[i] == true) linecount++;
    for(size_t i = 0; i < NUMLines; i++) linemem[i] = line_memory[i];
    for(size_t i = 0; i < NUMLines; i++) {
        // linecountが1以下の場合はループしないようにする
        size_t loop_count = (linecount > 1) ? (linecount - 1) : 0;
        for(size_t j = 0; j < loop_count; j++)
            linemem2[j] = calculate_deg('A', linemem[j], linemem[j+1]);
        for(size_t j = 0; j < NUMLines; j++)
            linemem[j] = linemem2[j];
        if(linecount > 0) linecount--;
    }
    return linemem[0];
}

int LINE::get_magnitude(void){
    read();
    if(read()== false){ //ラインが検出されていない場合
        return 999; //エラー値を返す
    };
    get_claster(); // ライン検出角度リストを取得　line_detectにクラスタがある
    if(count == 0) return 999; // ラインが検出されていない場合
    switch(count){
        case 1:
        return sensordist;
        break;
        case 2:
        return get_dist(line_detect[0], line_detect[1], sensordist);
        break;
        case 3:
        // int temp1[2];
        // int temp2[2];
        // temp1[0] = get_dist(line_detect[0], line_detect[1], sensordist);
        // temp1[1] = get_dist(line_detect[1], line_detect[2], sensordist);
        // temp2[0] = cal_deg('A', line_detect[0], line_detect[1]);
        // temp2[1] = cal_deg('A', line_detect[1], line_detect[2]);
        break;
        case 4:
        break;
        case 5:
        break;
        default:
        return 999; // ラインが検出されていない場合
    }
    return 999;
}

void LINE::get_claster(void){
    int cluster_deg=0;
    count = 0;
    for (size_t i = 0; i < NUMLines; i++){
        if(line_status[i])
        for(size_t j=i; j<NUMLines; j++){
            if(line_status[j+1]){
                cluster_deg = calculate_deg('A', cluster_deg, Line_deg_list_24[j+1]);
            }else{
                line_detect[count] = cluster_deg; // ライン検出角度リストに追加
                count++;
                cluster_deg = 0; // クラスタの角度をリセット
                i = j; // 次のライン検出角度を探すためにiを更新
                break;
            }
        }
    }
}

int LINE::get_dist(int deg1,int deg2,int dist){
    int theata=cal_deg('s',cal_deg('A',deg1,deg2),deg1);
    return cos(radians(theata))*dist;
}

bool LINE::read(void){

    for(size_t i=0; i<NUMLines; i++){//初期化
        line_status[i] = false;
        line_value [i] = 0;
        line_memory[i] = 0;
    }

    for (size_t i =0 ; i<3;i++){
        for(size_t j=0; j<NUMLines; j++){

            uint8_t pin =0;//ピン保存用
                switch (j%3) {//ピン選択
                    case 0: pin = readPin1; break;
                    case 1: pin = readPin2; break;
                    case 2: pin = readPin3; break;}

            // 選択ピンに出力
            digitalWrite(selectA, BinaryNum[j][0]);
            digitalWrite(selectB, BinaryNum[j][1]);
            digitalWrite(selectC, BinaryNum[j][2]);

            if(analogRead(pin) > lineDetect)line_value[j]++;

            line_memory[j] = analogRead(pin);//ラインの値をそのまま保存
        }
    }
    bool line_bool = false; // ライン検出フラグの初期化
        for (size_t i = 0; i < NUMLines; i++) {
            if (line_value[i] > 1) { // 2回以上検出されたらラインあり
                line_status[i] = true;
                line_bool = true;
            }else{line_status[i] = false;}
        }
    return line_bool; // ラインが検出されたかどうかを返す
}
