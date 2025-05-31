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

// 伝言
// りくはTODO書くな　邪魔だし紛らわしい　
// ARDUINOに入れるときに消すから勝手に消さないで　めっちゃ困る
// 勝手に変えたところ全部戻して
// ふざけんな　迷惑

// これから「無駄に」関与しないで

void LINE::setup() {
    pinMode(selectA, OUTPUT);
    pinMode(selectB, OUTPUT);
    pinMode(selectC, OUTPUT);
    pinMode(readPin1, INPUT);
    pinMode(readPin2, INPUT);
    pinMode(readPin3, INPUT);
}

int LINE::get_azimuth() {
    read();
    return get_linedeg();
}

int LINE::get_dist() {
    read();
    get_linedeg();
    if(count==1){
        return sensordist;
    }else if(count==2){
        return get_line_dist(line_detect[0], line_detect[1]);
    }else if(count==3){
        return get_line_dist(line_detect[0], line_detect[2]);
    }else if(count==4){
        return get_line_dist(line_detect[1], line_detect[2]);
    };
    return 999; // エラー用
}

bool LINE::read(){ //読み取りを24かいを三回繰り返して当たっていたら配列に１足して　２以上でboolをtrue
    for(int i=0; i<NUMLines; i++){ //初期化
        line_status[i] = false;
        line_detect[i] = 999;
        line_value [i] = 0;
    }

    int progress = 0;
    for(int j=0; j<3; j++){ // 3回繰り返し
        for(int k=0; k<3; k++){ // k: 0=readPin1, 1=readPin2, 2=readPin3
            int pin;//ピン処理
            if(k==0) pin = readPin1;
            else if(k==1) pin = readPin2;
            else pin = readPin3;
            for(int i=0; i<8; i++){
                int idx = k*8 + i;//idk=インデックス
                digitalWrite(selectA, BinaryNum[idx][0]);
                digitalWrite(selectB, BinaryNum[idx][1]);
                digitalWrite(selectC, BinaryNum[idx][2]);
                delay(1);
                if(analogRead(pin) > lineDetect){
                    line_value[idx]++;
                }
                progress++;
            }
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
            int max_idx = i;
            int max_val = line_value[i];
            int end = i;
            // クラスタの終端を探す
            for(int k = i; k < NUMLines && line_status[k] == true; k++) {
                sum += Line_deg_list_24[k];
                n++;
                if(line_value[k] > max_val) {
                    max_val = line_value[k];
                    max_idx = k;
                }
                end = k;
            }
            if (n > 0) {
                int avg = sum / n;
                int max_deg = Line_deg_list_24[max_idx];
                int diff = max_deg - avg;
                // 5度だけmax_idx方向に寄せる
                if(diff > 0) {
                    avg += (diff > 5 ? 5 : diff);
                } else if(diff < 0) {
                    avg += (diff < -5 ? -5 : diff);
                }
                line_detect[count] = avg;
                count++;
            }
            i = end + 1; // 次のクラスタへ
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


int LINE::get_line_dist(int linedeg ,int linedeg2){
    int linedist = 0;
    int theata=calculate_deg('s',linedeg2, linedeg);
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
    else if(mode=='r')//反転（fast only）
    {//num1を180度回転　　180にnum1を引いた絶対値を返す
        r=abs(180 - num1);
        r = num1;
    }
    else if(mode=='s')//減算（fast）
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