#include <Line.h>
#include <Input.h>
#include <Output.h>


#define lineDetect 5

//TODO消した　あいつはいいやつだったよ…（？）

void LINE::setup(void) {
    pinMode(selectA, OUTPUT);
    pinMode(selectB, OUTPUT);
    pinMode(selectC, OUTPUT);
    pinMode(readPin1, INPUT);
    pinMode(readPin2, INPUT);
    pinMode(readPin3, INPUT);
}

int LINE::get_azimuth(void) {
    bool linebool = read();
    return get_linedeg();
}

int LINE::get_magnitude(void){
    read();
    if(read()== false){ //ラインが検出されていない場合
        return 999; //エラー値を返す
    }
    get_linedeg();
    if(count==1){
        get_line_dist(line_detect[0],999);
    }else if(count==2){
        get_line_dist(line_detect[0], line_detect[1]);
    }else if(count==3){
        get_line_dist(line_detect[0], line_detect[2]);
    }else if(count==4){
        get_line_dist(line_detect[1], line_detect[2]);
}
return (myvector.get_magnitude(returnX,returnY)); // 座標から距離を取得
}

bool LINE::read(void){ //読み取りを24かいを三回繰り返して当たっていたら配列に１足して　２以上でboolをtrue
    for(int i=0; i<NUMLines; i++){ //初期化
        line_status[i] = false;
        line_detect[i] = 999;
        line_value [i] = 0;
    }

    int progress = 0;
    for(uint8_t  j=0; j<3; j++){ // 3回繰り返し
        for(uint8_t k=0; k<3; k++){ // k: 0=readPin1, 1=readPin2, 2=readPin3
            uint8_t  pin;//ピン処理
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

int LINE::get_linedeg(void) {
    // line_detectの初期化
    for(int i = 0; i < NUMLines; i++) {
        line_detect[i] = 999;
    }
    count = 0;
    int sensorIndex = 0;
    while (sensorIndex < NUMLines) {
        if (line_status[sensorIndex] == true) {
            int degreeSum = 0; // クラスタ内の角度合計
            int clusterCount = 0; // クラスタ内の要素数
            int maxValueIndex = sensorIndex; // 最大値のインデックス
            int maxSensorValue = line_value[sensorIndex]; // クラスタ内の最大センサ値
            int clusterEndIndex = sensorIndex;
            // クラスタの終端を探す
            for(int k = sensorIndex; k < NUMLines && line_status[k] == true; k++) {
                degreeSum += Line_deg_list_24[k];
                clusterCount++;
                if(line_value[k] > maxSensorValue) {
                    maxSensorValue = line_value[k];
                    maxValueIndex = k;
                }
                clusterEndIndex = k;
            }
            if (clusterCount > 0) {
                int clusterAverageDegree = degreeSum / clusterCount;
                int maxDegree = Line_deg_list_24[maxValueIndex];
                int diff = maxDegree - clusterAverageDegree;
                // 最大値方向に5度だけ寄せる
                if(diff > 5) {
                    clusterAverageDegree += 5;
                } else if(diff < -5) {
                    clusterAverageDegree -= 5;
                } else {
                    clusterAverageDegree += diff;
                }
                line_detect[count] = clusterAverageDegree;
                count++;
            }
            sensorIndex = clusterEndIndex + 1; // 次のクラスタへ
        } else {
            sensorIndex++;
        }
    }
    if(count == 0) {
        return 999;
    }
    int totalDegree = 0;
    for(int j = 0; j < count; j++) {
        totalDegree += line_detect[j];
    }
    // 複数クラスタならその平均
    return totalDegree / count;
    /*
      概要:
      - line_status配列から連続したtrueの範囲（クラスタ）を検出し、その範囲の角度平均を算出
      - クラスタ内で最も強いセンサ値方向に最大5度寄せて代表角度を決定
      - 複数クラスタがある場合はその平均値を返す
      - クラスタがなければ999（エラー値）を返す
    */
}


void LINE::get_line_dist(int linedeg ,int linedeg2){
    int linedist = 0;
    int theata=calculate_deg('s',linedeg2, linedeg);
    if(linedeg2 == 999)
    {
        linedist = sensordist;
    }
    else{
    linedist=cos(radians(theata))*sensordist;
    }
    myvector.get_cord(linedeg, linedist);
    returnX = myvector.get_x();
    returnY = myvector.get_y();
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