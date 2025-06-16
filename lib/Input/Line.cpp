#define RCJJ_VEGA 1
#include "line.h"
#include"okomeonigiri.h"
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
    read();
    get_claster();
    mypixel.closest(get_azimuth(),0,255,100,1);
    printf_s("line%d          %d\n",read(),get_azimuth());
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
    floop(NUMLines)if(line_status[i] == true) linecount++;
    floop(NUMLines)linemem[i] = line_memory[i];
    for(size_t i = 0; i < NUMLines; i++) {
        // linecountが1以下の場合はループしないようにする
        size_t loop_count = (linecount > 1) ? (linecount - 1) : 0;
        for(size_t j = 0; j < loop_count; j++)linemem2[j] = calculate_deg('A', linemem[j], linemem[j+1]);
        for(size_t j = 0; j < NUMLines; j++)linemem[j] = linemem2[j];
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
        int temp1[2];
        int temp2[2];
        temp1[0] = get_dist(line_detect[0], line_detect[1], sensordist);
        temp1[1] = get_dist(line_detect[1], line_detect[2], sensordist);
        temp2[0] = cal_deg('A', line_detect[0], line_detect[1]);
        temp2[1] = cal_deg('A', line_detect[1], line_detect[2]);
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
        if(line_status[i]){
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
}

int LINE::get_dist(int deg1,int deg2,int dist){
    int theata=cal_deg('s',cal_deg('A',deg1,deg2),deg1);
    return cos(radians(theata))*dist;
}





bool LINE::read(void){
    // 初期化
    for(size_t i=0; i<NUMLines; i++){
        line_status[i] = false;
        line_value [i] = 0;
        line_memory[i] = 0;
    }
// // readPin1 (0〜7)
// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[0] = analogRead(readPin1);


// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[1] = analogRead(readPin1);


// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[2] = analogRead(readPin1);


// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[3] = analogRead(readPin1);


// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[4] = analogRead(readPin1);


// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[5] = analogRead(readPin1);


// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[6] = analogRead(readPin1);


// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[7] = analogRead(readPin1);


// delay(1);

// // readPin2 (8〜15)
// digitalWrite(selectA, LOW);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[8] = analogRead(readPin2);

// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[9] = analogRead(readPin2);

// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[10] = analogRead(readPin2);

// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[11] = analogRead(readPin2);

// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[12] = analogRead(readPin2);

// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[13] = analogRead(readPin2);

// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[14] = analogRead(readPin2);

// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[15] = analogRead(readPin2);

// delay(1);

// // readPin3 (16〜23)
// digitalWrite(selectA, LOW);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[16] = analogRead(readPin3);

// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[17] = analogRead(readPin3);

// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[18] = analogRead(readPin3);

// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, LOW);
// delay(1);
// line_memory[19] = analogRead(readPin3);

// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[20] = analogRead(readPin3);

// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, LOW);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[21] = analogRead(readPin3);

// delay(1);

// digitalWrite(selectA, LOW);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[22] = analogRead(readPin3);

// delay(1);

// digitalWrite(selectA, HIGH);
// digitalWrite(selectB, HIGH);
// digitalWrite(selectC, HIGH);
// delay(1);
// line_memory[23] = analogRead(readPin3);

floop(NUMLines)if(line_memory[i] >= 500) {
        line_value[i]++;
        line_status[i] = true; // ラインが検出された場合
        mypixel.closest(Line_deg_list_24[i],0,0,255,1); // ラインの角度を表示
    } else {
        line_status[i] = false; // ラインが検出されなかった場合
    }


    // 読み取りには関係ないゾーン
    bool line_bool = false;
    for(int i=0; i<NUMLines; i++) if(line_status[i]) line_bool = true;
    for(int j=0; j<NUMLines; j++) { printf_s("%d  ",line_memory[j]); }
    printf_s("\n");
    for(int j=0; j<NUMLines; j++) { printf_s("%d  ",line_status[j]); }
    printf_s("\n");
    printf_s("\n");
    return line_bool;
    return 999;
}

int calculate_deg(char mode, int num1, int num2){
    switch(mode){
        case 'a': return (num1+num2>=360)?(num1+num2)%360:(num1+num2); // 加算
        case 'r': return (num1+180)%360; // 180度回転
        case 's': return (num1-num2<0)?(num1-num2+360)%360:(num1-num2); // 減算
        case 'A': return ((num1+num2)/2>=360)?((num1+num2)/2)%360:((num1+num2)/2); // 平均
        case 'm': return (num1<num2)?num1:num2; // 最小値
        case 'M': return (num1>num2)?num1:num2; // 最大値
        case 'd': return (num1-num2>=0)?(num1-num2):(num2-num1); // 差の絶対値
        case 'n': return (num1%360+360)%360; // 正規化
        case 'z': return (450-num1)%360; // azimuth→theta変換（num1のみ使用）
        case 't': return (450-num1)%360; // theta→azimuth変換（num1のみ使用）
        case 'q': return (num1+90)%360; // 90度回転（num1のみ使用）
        case 'Q': return (num1+270)%360; // -90度回転（num1のみ使用）
        default: return 999;
}}