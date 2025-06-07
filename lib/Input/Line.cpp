#include <Line.h>
#include <Input.h>
#include <Output.h>

#define lineDetect 500

//TODO消した　あいつはいいやつだったよ…（？）

void LINE::setup(void) {
    pinMode(selectA, OUTPUT);
    pinMode(selectB, OUTPUT);
    pinMode(selectC, OUTPUT);
    pinMode(readPin1, INPUT);
    pinMode(readPin2, INPUT);
    pinMode(readPin3, INPUT);
}

void LINE::serial_print(void){}

int LINE::get_azimuth(void) {
    read();
    for(uint8_t i = 0; i < 4; i++)line_detect[i] = 999; // ライン検出配列の初期化
    int linecount =0;
    int linemem[NUMLines] = {0};
    int linemem2[NUMLines] = {0};
    for(uint8_t i = 0; i < NUMLines; i++) if(line_status[i] == true) linecount++;
    for(uint8_t i = 0; i < NUMLines; i++) linemem[i] = line_memory[i];
    for(uint8_t i = 0; i < NUMLines; i++){
            for(uint8_t j =0; j<(linecount-1); j++) linemem2[j]=calculate_deg('A',linemem[j],linemem[j+1]);
            for(uint8_t j =0; j<NUMLines; j++)linemem[j]=linemem2[j];
            linecount--;
    }
    return linemem[0];
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

bool LINE::read(void){

    for(int i=0; i<NUMLines; i++){//初期化
        line_status[i] = false;
        line_value [i] = 0;
        line_memory[i] = 0;
    }

    for (uint8_t i =0 ; i<3;i++){
        for(uint8_t j=0; j<NUMLines; j++){

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
        for (uint8_t i = 0; i < NUMLines; i++) {
            if (line_value[i] >= 2) { // 2回以上検出されたらラインあり
                line_status[i] = true;
                line_bool = true;
            }else{line_status[i] = false;}
        }
    return line_bool; // ラインが検出されたかどうかを返す
}

int LINE::calculate_deg(char mode, int num1, int num2){switch(mode){
        case 'a': return (num1+num2>=360)?(num1+num2)%360:(num1+num2);
        case 'r': return (num1+180)%360;
        case 's': return (num1-num2<0)?(num1-num2+360)%360:(num1-num2);
        case 'A': return ((num1+num2)/2>=360)?((num1+num2)/2)%360:((num1+num2)/2);
        default: return 999;}}