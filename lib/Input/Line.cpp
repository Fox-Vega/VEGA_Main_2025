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

void LINE::serial_print(void) {
    read();
    for(uint8_t i = 0; i < NUMLines; i++) {
        Serial.print("line_status[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(line_status[i]);
        Serial.print(", line[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(line_memory[i]);
    }
}

int LINE::get_azimuth(void) {
    read();
    return get_linedeg();
}
int LINE::get_reverse_azimuth(void){
    int azimuth = get_azimuth();
    return cal_deg('r',azimuth,999);
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
//     for(int i=0; i<NUMLines; i++){ //初期化
//         line_status[i] = false;
//         line_detect[i] = 999;
//         line_value [i] = 0;
//     }

//     int progress = 0;
//     for(uint8_t  j=0; j<3; j++){ // 3回繰り返し
//         for(uint8_t k=0; k<3; k++){ // k: 0=readPin1, 1=readPin2, 2=readPin3
//             uint8_t  pin;//ピン処理
//             if(k==0) pin = readPin1;
//             else if(k==1) pin = readPin2;
//             else pin = readPin3;
//             for(int i=0; i<8; i++){
//                 int idx = k*8 + i;//idk=インデックス
//                 digitalWrite(selectA, BinaryNum[idx][0]);
//                 digitalWrite(selectB, BinaryNum[idx][1]);
//                 digitalWrite(selectC, BinaryNum[idx][2]);
//                 delay(1);
//                 if(analogRead(pin) > lineDetect){
//                     line_value[idx]++;
//                 }
//                 progress++;
//             }
//         }
//     }
//     bool line_bool =0;
//     for(int i = 0; i < 24; i++){
//         if(line_value[i] >= 2){
//             line_status[i] = true;
//             line_bool = 1;
//         }
//         else{
//             line_status[i] = false;
//         }
//         progress++;
//     }
//     if(line_bool == 1){
//         return true;
//     }
//     else{
//         return false;
//     }
// }


    for(int i=0; i<NUMLines; i++){ //初期化
        line_status[i] = false;
        line_value [i] = 0;
        line_memory[i] = 0; // ライン検出の履歴
    }

    for (uint8_t i =0 ; i<3;i++){
        for(uint8_t j=0; j<NUMLines; j++){
            uint8_t pin =0;//ピン変数
            switch (j%3) {//ピン処理
                case 0: pin = readPin1; break;
                case 1: pin = readPin2; break;
                case 2: pin = readPin3; break;
            }
            // バイナリ配列の値を選択ピンに出力
            digitalWrite(selectA, BinaryNum[j][0]);
            digitalWrite(selectB, BinaryNum[j][1]);
            digitalWrite(selectC, BinaryNum[j][2]);
            //少し待つ（安定化）
            delay(1);
            if(analogRead(pin) > lineDetect){//アナログ読み取ってしきい値でふるいにかける
                line_value[j]++;
            }
            line_memory[j] = analogRead(pin); // ライン検出の履歴を保存
        }
    }
    bool line_bool = false; // ライン検出フラグの初期化
    for (uint8_t i = 0; i < NUMLines; i++) {
        if (line_value[i] >= 2) { // 2回以上検出されたらラインあり
            line_status[i] = true;
            line_bool = true; // ラインが検出された
        } else {
            line_status[i] = false;
        }
    }
    return line_bool; // ラインが検出されたかどうかを返す
}

void LINE::get_claster(void){
        count = 0; // ライン検出数の初期化
    for(uint8_t i = 0; i<NUMLines; i++){
        if(line_status[i]==true)
        {
            int first = i;
            int last = i;
            for(uint8_t j = i; j < NUMLines; j++) {
                last = j; // ラインが続いている限りlastを更新
                if(line_status[j] == false) {
                    int degreeSum = 0; // クラスタ内の角度合計
                    for(uint8_t k = first; k <= last; k++) {
                        degreeSum += Line_deg_list_24[k];
                    }
                    int clusterAverageDegree = degreeSum / (last - first + 1); // クラスタの平均角度
                    line_detect[count] = clusterAverageDegree; // 平均角度をライン検出配列に格納
                    count++; // ライン検出数を増やす
                    break; // ラインが途切れたらループを抜ける
                }
            }
            i = last; // 次のライン検出の開始位置を更新
        }
    }
}

int LINE::get_linedeg(void){
    for(uint8_t i = 0; i < 4; i++) {
        line_detect[i] = 999; // ライン検出配列の初期化
    }
    get_claster();
    switch(count) {
        case 0:linesituation = 0; // ラインなし
        case 1:linesituation = 1;//点
        case 2:linesituation = 2; // 直線
        case 3:linesituation = 3;// 曲線
        case 4:linesituation = 4;// 角
        default: linesituation = count;//それ以上
    }
    if(linesituation==0)
    {return 999; // ラインなしの場合は999を返す
    } else if(linesituation==1) {
        return line_detect[0]; // 点の場合はその角度を返す
    } else if(linesituation==2) {
        return cal_deg('A', line_detect[0], line_detect[1]); // 直線の場合は平均角度を返す
    } else if(linesituation==3) {
        int line1 =0;
        int line2 =0;
        line1 = cal_deg('s', line_detect[0], line_detect[1]);
        line2 = cal_deg('s', line_detect[1], line_detect[2]);
        return cal_deg('A', line1, line2);
    } else if(linesituation==4) {
        int line1 =0;
        int line2 =0;
        int line3 =0;
        line1 = cal_deg('s', line_detect[0], line_detect[1]);
        line2 = cal_deg('s', line_detect[1], line_detect[2]);
        line3 = cal_deg('s', line_detect[2], line_detect[3]);
        return cal_deg('A', line1, cal_deg('A', line2, line3)); // 角の場合は平均角度を返す
    } else if(count%2==0)
        {
            int lined1 =linesituation/2;
            int lined2 =linesituation/2+1;
            return cal_deg('A', line_detect[lined1], line_detect[lined2]);
        }
        else
        {
            int lined1 =(linesituation+1)/2;
            return line_detect[lined1];
        };
    };
//     // line_detectの初期化
//     for(int i = 0; i < NUMLines; i++) {
//         line_detect[i] = 999;
//     }
//     count = 0;
//     int sensorIndex = 0;
//     while (sensorIndex < NUMLines) {
//         if (line_status[sensorIndex] == true) {
//             int degreeSum = 0; // クラスタ内の角度合計
//             int clusterCount = 0; // クラスタ内の要素数
//             int maxValueIndex = sensorIndex; // 最大値のインデックス
//             int maxSensorValue = line_value[sensorIndex]; // クラスタ内の最大センサ値
//             int clusterEndIndex = sensorIndex;
//             // クラスタの終端を探す
//             for(int k = sensorIndex; k < NUMLines && line_status[k] == true; k++) {
//                 degreeSum += Line_deg_list_24[k];
//                 clusterCount++;
//                 if(line_value[k] > maxSensorValue) {
//                     maxSensorValue = line_value[k];
//                     maxValueIndex = k;
//                 }
//                 clusterEndIndex = k;
//             }
//             if (clusterCount > 0) {
//                 int clusterAverageDegree = degreeSum / clusterCount;
//                 int maxDegree = Line_deg_list_24[maxValueIndex];
//                 int diff = maxDegree - clusterAverageDegree;
//                 // 最大値方向に5度だけ寄せる
//                 if(diff > 5) {
//                     clusterAverageDegree += 5;
//                 } else if(diff < -5) {
//                     clusterAverageDegree -= 5;
//                 } else {
//                     clusterAverageDegree += diff;
//                 }
//                 line_detect[count] = clusterAverageDegree;
//                 count++;
//             }
//             sensorIndex = clusterEndIndex + 1; // 次のクラスタへ
//         } else {
//             sensorIndex++;
//         }
//     }
//     if(count == 0) {
//         return 999;
//     }
//     int totalDegree = 0;
//     for(int j = 0; j < count; j++) {
//         totalDegree += line_detect[j];
//     }
//     // 複数クラスタならその平均
//     return totalDegree / count;
//     /*
//       概要:
//       - line_status配列から連続したtrueの範囲（クラスタ）を検出し、その範囲の角度平均を算出
//       - クラスタ内で最も強いセンサ値方向に最大5度寄せて代表角度を決定
//       - 複数クラスタがある場合はその平均値を返す
//       - クラスタがなければ999（エラー値）を返す
//     */


/*void LINE::get_line_dist(int linedeg ,int linedeg2){
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
*/
int LINE::calculate_deg(char mode, int num1, int num2){
    switch(mode){
        case 'a': return (num1+num2>=360)?(num1+num2)%360:(num1+num2);
        case 'r': return (num1+180)%360;
        case 's': return (num1-num2<0)?(num1-num2+360)%360:(num1-num2);
        case 'A': return ((num1+num2)/2>=360)?((num1+num2)/2)%360:((num1+num2)/2);
        default: return 999;
    }
}