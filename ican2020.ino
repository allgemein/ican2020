/*
   ICAN2020
   QSHP制御コード
*/
#include <Wire.h>
#include <Regexp.h>
#include <SSD1306.h>
#include <BluetoothSerial.h>

#if defined ( ESP32 )
HardwareSerial  GpsSerial( 2 );
#else
#define  GpsSerial  Serial1;
#endif

#define MMA8452_ADRS 0x1D
#define MMA8452_OUT_X_MSB 0x01
#define MMA8452_XYZ_DATA_CFG 0x0E
#define MMA8452_CTRL_REG1 0x2A
#define MMA8452_CTRL_REG1_ACTV_BIT 0x01
#define MMA8452_G_SCALE 2

BluetoothSerial bt;
SSD1306  display(0x3c, 23, 22);

long long  headAccelSum[3]={0,0,0};
unsigned long long counter=0;

typedef struct {
  float x;
  float y;
  float z;
} vec3;

const float k = 0.9;
const float threshold = 100; 
int fontHeight = 15;
float pulse; //心拍数

void setup() {
  display.init();
  display.setFont(ArialMT_Plain_24);
  display.drawString(15, 15, "QSHP!");
  display.display();
  display.setFont(ArialMT_Plain_16);
  accelInit();
  bt.begin("QSHP");
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  while (!Serial1)delay(200);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13,LOW);
}

void loop() {
  float temp;
  vec3 None = { -1, -1, -1};
  vec3 headAccel, wristAccel;
  static vec3 previousHeadAccel;
  if (Serial1.available()) {
	  /*センサの値を取得*/
	  if  (headAccel.x != None.x && headAccel.y != None.y && headAccel.z != None.z) {
		  headAccel = lowPassFilter(previousHeadAccel,getHeadAcclerelation());
		  previousHeadAccel = headAccel;
		  temp = getTemp();
		  wristAccel = getWristAcclerelation();
		  if (isNeochi(headAccel, wristAccel, temp, pulse)) {
         /*寝る落ちしてるか?*/
        vaible();//バイブレーション
		bt.println("neochi");
      }
      //ここからディスプレイ表示用
      display.clear();
      String Show = "Temp:";
      Show.concat(String(temp));
      display.drawString(0, fontHeight * 0, Show);
      String Output = "Head:";
      Output.concat(String((int)headAccel.x));
      Output.concat(",");
      Output.concat(String((int)headAccel.y));
      Output.concat(",");
      Output.concat(String((int)headAccel.z));
      display.drawString(0, fontHeight * 1, Output);
      String Output2 = "";
      Output2.concat(String(wristAccel.x));
      Output2.concat(",");
      Output2.concat(String(wristAccel.y));
      Output2.concat(",");
      Output2.concat(String(wristAccel.z));
      display.drawString(0, fontHeight * 2, Output2);
      display.display();
    }
  }
}
