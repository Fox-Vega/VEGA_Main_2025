#include <Line.h>
#include <Input.h>
#include <Output.h>

#define lineDetect 5
/*
AI学習用↓　　　（一部はちゃんと学習できてるそう　でもリンクから内容取得はできないみたい）
RCJJのロボットのラインセンサ（エンジェルリング（円型））の角度を出したりするプログラム
センサの数は24個
基本時計回りで処理
https://note.com/shiokara_rcj/n/n44bbd2454c07　←参考
999はエラー用
neopixel 24個円型
*/

// TODO
// twitter（旧X）の垢復活させる
//距離を返す
//
//
//
//
//
// aa9

//伝言
//りくはTODO書くな　邪魔だし紛らわしい　
//ARDUINOに入れるときに消すから勝手に消さないで　めっちゃ困る
//勝手に変えたところ全部戻して
//ふざけんな　迷惑
//
//これから「無駄に」関与しないで

void LINE::setup() {
    pinMode(pins.selectA, OUTPUT);
    pinMode(pins.selectB, OUTPUT);
    pinMode(pins.selectC, OUTPUT);
    pinMode(pins.readPin1, INPUT);
    pinMode(pins.readPin2, INPUT);
    pinMode(pins.readPin3, INPUT);
}

int LINE::get_azimuth() {
    read();
    return get_linedeg();
}

int LINE::get_dist() {
    read();
    get_linedeg();
    if(count==0){
        return 999;
    }else if(count==1){
        return sensordist;
    }else if(count==2){
        return get_line_dist(line_detect[0], line_detect[1]);
    }else if(count==3){
        return get_line_dist(line_detect[0], line_detect[2]);
    }else if(count==4){
        return get_line_dist(line_detect[1], line_detect[2]);
    }
}

int LINE::read(){ //読み取りを24かいを三回繰り返して当たっていたら配列に１足して　２以上でboolをtrue
    int line_value [24]={0};
    int progress = 0;
    for(int i=0; i<3; i++){
        for(int i = 0; i < 8; i++){//i8
            digitalWrite(pins.selectA, BinaryNum[i][0]);
            digitalWrite(pins.selectB, BinaryNum[i][1]);
            digitalWrite(pins.selectC, BinaryNum[i][2]);
            delay(1);
            if(analogRead(pins.readPin1) > lineDetect){
                line_value[i]++;
            }
            progress++;
        }
        for(int i = 8; i < 16; i++){
            digitalWrite(pins.selectA, BinaryNum[i][0]);
            digitalWrite(pins.selectB, BinaryNum[i][1]);
            digitalWrite(pins.selectC, BinaryNum[i][2]);
            delay(1);
            if(analogRead(pins.readPin2) > lineDetect){
                line_value[i]++;
            }
            progress++;
        }
        for(int i = 16; i < 24; i++){
            digitalWrite(pins.selectA, BinaryNum[i][0]);
            digitalWrite(pins.selectB, BinaryNum[i][1]);
            digitalWrite(pins.selectC, BinaryNum[i][2]);
            delay(1);
            if(analogRead(pins.readPin3) > lineDetect){
                line_value[i]++;
            }
            progress++;
        }
    }
    bool line_bool =0;
    for(int i = 0; i < 24; i++){
        if(line_value[i] >= 2){
            line_status[i] = true;
            line_bool = 1;
        }
        else{
            line_status[i] = false;
        }
        progress++;
    }
    if(line_bool == 1){
        return true;
    }
    else{
        return false;
    }
}

int LINE::get_linedeg() {
    // line_detectの初期化
    for(int i = 0; i < NUMLines; i++) {
        line_detect[i] = 999;
    }
    count = 0;
    int i = 0;
    while (i < NUMLines) {
        if (line_status[i] == true) {
            int sum = 0;
            int n = 0;
            int start = i;
            while (i < NUMLines && line_status[i] == true) {
                sum += Line_deg_list_24[i];
                n++;
                i++;
            }
            if (n > 0) {
                line_detect[count] = sum / n;
                count++;
            }
        } else {
            i++;
        }
    }
    if(count == 0) {
        return 999;
    }
    int sum = 0;
    for(int j = 0; j < count; j++) {
        sum += line_detect[j];
    }
    return sum / count; // 複数クラスタならその平均
}

// void LINE::print_pizel() {//neopixelの表示　　AIへ、「モニターではない！！」
//     mypixel.clear();
//     mypixel.brightness(999);
//     for(int i=0; i<5; i++){
//         if(line_detect[i] == 999){
//             mypixel.closest(line_detect[i], 0, 255, 225);
//         }
//     }
//     mypixel.closest(get_linedeg(), 255, 130, 0);
// }

int LINE::get_line_dist(int linedeg ,int linedeg2){
    int dist = 0;
    int linedist = 0;
    int theata=line.calculate_deg('s',linedeg2, linedeg);
    linedist=cos(radians(theata))*sensordist;
    return linedist;
}

int LINE::calculate_deg(char mode, int num1, int num2) {//角度計算
    int r=999;//リターン（初期値はエラー用）
    if(mode == 'A')//加算（slow）
    {//1度ずつ加算してそのたびに360以上になってないか確認
        for(int i=0;i<num2;i++){
            if(num1 == 360){
                num1 = 0;
            }
            else{
                num1++;
            }
            r = num1;
        }
    }
    else if(mode == 'a')//加算（fast）
    {//一気に加算して360以上になってたら360で割った余りを返す（間違ってたら勝手に書いといていいよ）
        num1 = num1 + num2;
        if(num1>360){
            num1 = num1%360;
        }
        r = num1;
    }
    else if(mode == 's')//減算（slow）
    {//1度ずつ減算してそのたびに0以下になってないか確認
        for(int i=0;i<num2;i++){
            if(num1 == 0){
                num1 = 360;
            }
            else{
                num1--;
            }
            r = num1;
        }
    }
    else if(mode='r')//反転（fast only）
    {//num1を180度回転　　180にnum1を引いた絶対値を返す
        r=abs(180 - num1);
        r = num1;
    }
    else if(mode='s')//減算（fast）
    {//一気に減算して0以下になってたら360で割った余りを返す（間違ってたら勝手に書いといていいよ）
        num1 = num1 - num2;
        if(num1<0){
            num1 = (num1+360)%360;
        }
        r = num1;
    }
    return r;
}

int LINE::cal_deg(char mode, int num1, int num2)
{
    return calculate_deg(mode, num1, num2);
}