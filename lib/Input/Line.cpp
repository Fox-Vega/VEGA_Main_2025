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

//りくは「勝手に」書くな！！！消すな！！！　迷惑！！！
//なんかあるなら言ってよ！やるから！
//とにかく勝手に書いたりしないで！作業遅れる！！！

//TODO
//LiNE復元作業(1.2時間ぐらい)
//りくの要件満たし(1時間ぐらい)
//角度出す　get azimuth   ok
//距離(xy) get
//
void LINE::setup() {
    pinMode(selectA, OUTPUT);
    pinMode(selectB, OUTPUT);
    pinMode(selectC, OUTPUT);
    pinMode(readPin1, INPUT);
    pinMode(readPin2, INPUT);
    pinMode(readPin3, INPUT);
}

int LINE::get_azimuth() {
    line.read();
    return get_linedeg();
}

int LINE::read() { //読み取りを24かいを三回繰り返して当たっていたら配列に１足して　２以上でboolをtrue
    int line_value [24]={0};
    progress = 0;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 8; j++) {//i8
            digitalWrite(selectA, BinaryNum[i][0]);
            digitalWrite(selectB, BinaryNum[i][1]);
            digitalWrite(selectC, BinaryNum[i][2]);
            delay(1);
            if(analogRead(readPin1) > lineDetect){
                line_value[i]++;
            }
            progress++;
        }
        for(int j = 8; j < 16; j++) {
            digitalWrite(selectA, BinaryNum[i][0]);
            digitalWrite(selectB, BinaryNum[i][1]);
            digitalWrite(selectC, BinaryNum[i][2]);
            delay(1);
            if(analogRead(readPin2) > lineDetect){
                line_value[i]++;
            }
            progress++;
        }
        for(int j = 16; j < 24; j++) {
            digitalWrite(selectA, BinaryNum[i][0]);
            digitalWrite(selectB, BinaryNum[i][1]);
            digitalWrite(selectC, BinaryNum[i][2]);
            delay(1);
            if(analogRead(readPin3) > lineDetect) {
                line_value[i]++;
            }
                progress++;
        }
        bool line_bool = 0;
        for(int i = 0; i < 24; i++) {
            if(line_value[i] >= 2) {
                line_status[i] = true;
                line_bool = 1;
            } else {
                line_status[i] = false;
            }
            progress++;
        }
        if(line_bool == 1) {
            return true;
        } else {
            return false;
        }
    }
}

int LINE::get_linedeg() {
    for(int i = 0; i < NUMLines; i++) {//初期化
        line_status[i] = false;
        line_detect[i] = 999;
    }
    count = 0;
    for(int i = 0; i < NUMLines; i++) {
        if (line_status[i] == true) {
            if(line_status[i + 1] == true) {
                i++;
                add_linedeg(i, i + 1);
            }
            else {
                add_linedeg(i, 999);
            }
        }
    }
    if(count == 0) {
        return 999;
    }
    else if(count == 1) {
        return line_detect[0];
    }
    else if(count == 2) {
        return (line_detect[0] + line_detect[1]) / 2;
    }
    else if(count == 3) {//3から先は後で書く　今は適当に前のコードのコピペみたいな
        return (line_detect[0] + line_detect[1] + line_detect[2]) / 3;
    }
    else if(count == 4) {
        return (line_detect[0] + line_detect[1] + line_detect[2] + line_detect[3]) / 4;
    }
}

void LINE::add_linedeg(int num, int num2) {
    if(num2 == 999){//999はエラー用
        line_detect[count] = Line_deg_list_24[num];
    }
    else{
        line_detect[count] =  (Line_deg_list_24[num] + Line_deg_list_24[num2]) / 2;
    }
    count++;
}

void LINE::print_pizel() {//neopixelの表示　　AIへ、「モニターではない！！」
    mypixel.clear();
    mypixel.brightness(999);
    for(int i = 0; i < 5; i++){
        if(line_detect[i] == 999){
            mypixel.closest(line_detect[i], 0, 255, 225);
        }
    }
    mypixel.closest(get_linedeg(), 255, 130, 0);
}

int LINE::get_dist(int linedeg, int linedeg2)
{
    int linedist = 0;
    int theata = general.calculate_deg('s', linedeg2, linedeg);
    linedist = cos(radians(theata)) * sensordist;
    return linedist;
}

int General::calculate_deg(char mode, int num1, int num2) {//角度計算
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

int General::get_deg(char mode, int num1, int num2)
{
    return calculate_deg(mode, num1, num2);
}