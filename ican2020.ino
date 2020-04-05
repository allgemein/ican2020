/*
   ICAN2020
   QSHP制御コード
 */
#include <Wire.h>
#include <Regexp.h>
#include <SSD1306.h>
#include <BluetoothSerial.h>
#include "fillter.h"

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

typedef struct vec{
	float x;
	float y;
	float z;
	vec operator+(const vec& v){
		vec ret;
		ret.x = x + v.x;
		ret.y = y + v.y;
		ret.z = z + v.z;
		return ret;
	}
	vec operator-(const vec& v){
		vec ret;
		ret.x = x - v.x;
		ret.y = y - v.y;
		ret.z = z - v.z;
		return ret;
	}
	vec operator*(const double d){
		vec ret;
		ret.x = d * x;
		ret.y = d * y;
		ret.z = d * z;
		return ret;
	}
	vec& operator+=(const vec& v){
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	vec& operator-=(const vec& v){
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
} vec3;

const vec3 None = { -1, -1, -1};
const float threshold = 100; 
int fontHeight = 15;
float pulse; //心拍数

lowPassFillter<vec3> headAccel(0.85);
bandPassFillter<vec3> wristAccel(0.9,0.3);

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
	wristAccel.setData(getWristAcclerelation());
	if (Serial1.available()) {
		/*センサの値を取得*/
		headAccel.setData(getHeadAcclerelation());
		if(1/*isNotNone*/){
			temp = getTemp();
			if (1/*isNeochi*/) {
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
			Output.concat(String((int)headAccel.getResult().x));
			Output.concat(",");
			Output.concat(String((int)headAccel.getResult().y));
			Output.concat(",");
			Output.concat(String((int)headAccel.getResult().z));
			display.drawString(0, fontHeight * 1, Output);
			String Output2 = "";
			Output2.concat(String(wristAccel.getResult().x));
			Output2.concat(",");
			Output2.concat(String(wristAccel.getResult().y));
			Output2.concat(",");
			Output2.concat(String(wristAccel.getResult().z));
			display.drawString(0, fontHeight * 2, Output2);
			display.display();
		}
		printVec3(wristAccel.getResult());
		printVec3ln(wristAccel.getData());
	}
}
