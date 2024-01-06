#include <Arduino.h>
#include <SCServo.h>
#include <HardwareSerial.h>

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel rawpixel(16, 7, NEO_GRB + NEO_KHZ800);

#include "./fled/fled.h"
FLED led(&rawpixel, 0, 15);

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
SMS_STS st;

#include "./motor/motor.h"
MOTOR motor(&Serial2, &st);

int val = 0;
int val1 = 0;
int val2 = 0;// 読み取った値を格納する変数

int MUX_A = A0;
int MUX_B = A1;

const int digitalPin[3] = {23, 24, 25};  // 任意のピン番号

void digitalOutBinary(int value) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(digitalPin[i], value & 1);
    value >>= 1;
  }
}

/*
  digitalOutBinary:A0/A1
  0:floor0/Red0
  1:floor1/Green0
  2:floor2/Bule0
  3:floor3/Red1
  4:PSD0/Green1
  5:PSD1/Bule1
  6:PSD2/LOAD_L
  7:PSD3/LOAD_R
*/

#define a 0.8   //0<a<1の範囲　大きいほど効果大

void setup(){
  bno.begin();
  Serial.begin(9600);
  Serial1.begin(9600);
  motor.init();
  motor.stop();

  pinMode(14,INPUT);

  led.init();
  delay(50);

  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);

  pinMode(MUX_A, INPUT);
  pinMode(MUX_B, INPUT);

  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);

  for(int i=10; i<1750; i=i+10){
    led.clear();
    led.set_color_hsv_all(230,250,20);
    led.set_width_hsv(15.0,i/100.0,150,250,i/10.0);
    led.show();
    delay(1);
 }
 delay(50);
}

uint32_t led_timer=0;
uint8_t led_status = 0;

uint32_t sec_timer = 0;
uint32_t sec_count = 0;

void loop(){
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  int gyro_degree = euler.x();
  // Serial.println(gyro_degree);
  led.clear();

  if(sec_timer + 2000 < millis()){
    sec_timer = millis();
    sec_count += 1;
  }

  if(led_timer + 500 < millis()){//timer
    led_timer = millis();
    led_status += 1;
    if(led_status == 5) led_status = 0;
  }
  
  if(!digitalRead(14)){
    if(sec_count%4 == 0){
      motor.right(4000,200);
      led.set_color_rgb_all(0,0,200);
    }else if(sec_count%4 == 2){
      motor.left(4000,200);
      led.set_color_rgb_all(0,200,0);
    }else{
      motor.stop();
      led.set_color_rgb_all(200,0,0);
    }
  }else{
    motor.stop();
    if(led_status%5 == 0){
      led.set_color_hsv_all(100,255,50);
    }else{
      led.set_color_hsv_all((led_status%5)*50,255,50);
    }
  }

  led.set_width_hsv(15.0+(gyro_degree*0.04444),3.5,150,200,255);
  led.show();


  // delay(1000);
  // Position[0] = 4095;
  // Position[1] = 4095;
  // st.SyncWritePosEx(ID, 2, Position, Speed, ACC);//舵机(ID1/ID2)以最高速度V=3400步/秒，加速度A=50(50*100步/秒^2)，运行至P1=4095位置
  // delay(1884);//((P1-P0)/V)*1000+(V/(A*100))*1000

  // Position[0] = 0;
  // Position[1] = 0;
  // st.SyncWritePosEx(ID, 2, Position, Speed, ACC);//舵机(ID1/ID2)以最高速度V=3400步/秒，加速度A=50(50*100步/秒^2)，运行至P0=0位置
  // delay(1884);//((P1-P0)/V)*1000+(V/(A*100))*1000

  digitalOutBinary(0);
  val1 = analogRead(A1);
  digitalOutBinary(3);
  val2 = analogRead(A1);
  float Rval = ((val1 + val2) / 2 ) * 1.5;
  float Rrc = 0;
  Rrc = a * Rrc + (1 - a) * (float)Rval;
  Serial.print('R');
  Serial.println(Rrc);

  digitalOutBinary(1);
  val1 = analogRead(A1);
  digitalOutBinary(4);
  val2 = analogRead(A1);
  float Gval = ( (val1 + val2) / 2) * 1.1;
  float Grc = 0;
  Grc = a * Grc + (1 - a) * (float)Gval;
  Serial.print('G');
  Serial.println(Grc);

  digitalOutBinary(2);
  val1 = analogRead(A1);
  digitalOutBinary(5);
  val2 = analogRead(A1);
  float Bval = ((val1 + val2) / 2);
  float Brc = 0;
  Brc = a * Brc + (1 - a) * (float)Bval;
  Serial.print('B');
  Serial.println(Brc);

  
    if (Rrc > Grc) {
     if (Rrc > Brc) {
       Serial.println("RED");
       Stop();
       //LED点滅
       delay(2000);
       forward();
       delay( 1000 );
       Stop();

     } else {
       Serial.println("WHITE");

       int i = 0;
       led.clear();
       while (i < 16) {
         pixels.setPixelColor(i, pixels.Color(0, 0, 0));
         pixels.show();
         delay(10);
         i++;

         digitalOutBinary(4);
         val = analogRead(A0);
         Serial.print("PSD");
         Serial.println(val);

         int PSDval = 0;
         if (PSDval < 300) {
           forward();
         } else {
           Stop();
           delay(1000);
           back();
           delay(300);
           Left() ;
           delay(800);
           Stop();
           delay(100);
           //back();
           delay(400);
           Stop();
           delay(100);
         }
       }
     }
    } else {
     if (Grc > Brc) {
       Serial.println("GREEN");
       Stop();
       //LED点滅
       delay(2000);
       forward();
       delay( 1000 );
       Stop();


     } else {
       Serial.println("WHITE");
       int i = 0;
       led.clear();
       while (i < 16) {
         pixels.setPixelColor(i, pixels.Color(0, 0, 0));
         pixels.show();
         delay(10);
         i++;

         int PSDval = 0;
         digitalOutBinary(4);
         PSDval = analogRead(A0);
         Serial.print("PSD");
         Serial.println(val);


         if (PSDval < 300) {
           forward();
         } else {
           Stop();
           delay(1000);
           back();
           delay(300);
           Left() ;
           delay(800);
           Stop();
           delay(100);
           //back();
           delay(400);
           Stop();
           delay(100);
         }
       }
     }
    }
  
}

void setup1(){
}

void loop1(){
} 















/*
#include <Adafruit_NeoPixel.h>
#define PIN        7
#define NUMPIXELS 16
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int val = 0;
int val1 = 0;
int val2 = 0;// 読み取った値を格納する変数

int MUX_A = A0;
int MUX_B = A1;

const int digitalPin[3] = {23, 24, 25};  // 任意のピン番号

void digitalOutBinary(int value) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(digitalPin[i], value & 1);
    value >>= 1;
  }
}

  digitalOutBinary:A0/A1
  0:floor0/Red0
  1:floor1/Green0
  2:floor2/Bule0
  3:floor3/Red1
  4:PSD0/Green1
  5:PSD1/Bule1
  6:PSD2/LOAD_L
  7:PSD3/LOAD_R




#define a 0.8   //0<a<1の範囲　大きいほど効果大

const int M0_A = 10;
const int M0_B = 11;
const int M1_A = 12;
const int M1_B = 13;

const int M2_A = 17;
const int M2_B = 16;
const int M3_A = 19;
const int M3_B = 18;

void forward() {
  analogWrite( M0_A, 0 );
  analogWrite( M0_B, 40 );
  analogWrite( M1_A, 0 );
  analogWrite( M1_B, 40 );
  analogWrite( M2_A, 0 );
  analogWrite( M2_B, 40 );
  analogWrite( M3_A, 0 );
  analogWrite( M3_B, 40 );
}

void Stop() {
  analogWrite( M0_A, 0 );
  analogWrite( M0_B, 0 );
  analogWrite( M1_A, 0 );
  analogWrite( M1_B, 0 );
  analogWrite( M2_A, 0 );
  analogWrite( M2_B, 0 );
  analogWrite( M3_A, 0 );
  analogWrite( M3_B, 0 );
}

void Left() {
  analogWrite( M0_A, 0 );
  analogWrite( M0_B, 70 );
  analogWrite( M1_A, 0 );
  analogWrite( M1_B, 70 );
  analogWrite( M2_A, 70 );
  analogWrite( M2_B, 0 );
  analogWrite( M3_A, 0 );
  analogWrite( M3_B, 70 );
}

void back() {
  analogWrite( M0_A, 40 );
  analogWrite( M0_B, 0 );
  analogWrite( M1_A, 40 );
  analogWrite( M1_B, 0 );
  analogWrite( M2_A, 40 );
  analogWrite( M2_B, 0 );
  analogWrite( M3_A, 40 );
  analogWrite( M3_B, 0 );
}



void setup() {
  pixels.begin();

  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);

  pinMode(MUX_A, INPUT);
  pinMode(MUX_B, INPUT);

  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);

  Serial.begin(9600);
}




void loop() {

  digitalOutBinary(0);
  val1 = analogRead(A1);
  digitalOutBinary(3);
  val2 = analogRead(A1);
  float Rval = ((val1 + val2) / 2 ) * 1.5;
  float Rrc = 0;
  Rrc = a * Rrc + (1 - a) * (float)Rval;
  Serial.print('R');
  Serial.println(Rrc);

  digitalOutBinary(1);
  val1 = analogRead(A1);
  digitalOutBinary(4);
  val2 = analogRead(A1);
  float Gval = ( (val1 + val2) / 2) * 1.1;
  float Grc = 0;
  Grc = a * Grc + (1 - a) * (float)Gval;
  Serial.print('G');
  Serial.println(Grc);

  digitalOutBinary(2);
  val1 = analogRead(A1);
  digitalOutBinary(5);
  val2 = analogRead(A1);
  float Bval = ((val1 + val2) / 2);
  float Brc = 0;
  Brc = a * Brc + (1 - a) * (float)Bval;
  Serial.print('B');
  Serial.println(Brc);

  //delay(1000);
  
    if (Rrc > Grc) {
     if (Rrc > Brc) {
       Serial.println("RED");
       Stop();

       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(255, 0, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
         delay(500);
       }
       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(255, 0, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
         delay(500);
       }
       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(255, 0, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
       }
       delay(2000);
       forward();
       delay( 1000 );
       Stop();

     } else {
       Serial.println("WHITE");

       int i = 0;
       pixels.clear();
       while (i < 16) {
         pixels.setPixelColor(i, pixels.Color(0, 0, 0));
         pixels.show();
         delay(10);
         i++;

         digitalOutBinary(4);
         val = analogRead(A0);
         Serial.print("PSD");
         Serial.println(val);

         int PSDval = 0;
         if (PSDval < 300) {
           forward();
         } else {
           Stop();
           delay(1000);
           back();
           delay(300);
           Left() ;
           delay(800);
           Stop();
           delay(100);
           //back();
           delay(400);
           Stop();
           delay(100);
         }
       }
     }
    } else {
     if (Grc > Brc) {
       Serial.println("GREEN");
       Stop();
       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 255, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
         delay(500);
       }

       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 255, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
         delay(500);
       }
       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 255, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
       }

       delay(2000);
       forward();
       delay( 1000 );
       Stop();


     } else {
       Serial.println("WHITE");
       int i = 0;
       pixels.clear();
       while (i < 16) {
         pixels.setPixelColor(i, pixels.Color(0, 0, 0));
         pixels.show();
         delay(10);
         i++;

         int PSDval = 0;
         digitalOutBinary(4);
         PSDval = analogRead(A0);
         Serial.print("PSD");
         Serial.println(val);


         if (PSDval < 300) {
           forward();
         } else {
           Stop();
           delay(1000);
           back();
           delay(300);
           Left() ;
           delay(800);
           Stop();
           delay(100);
           //back();
           delay(400);
           Stop();
           delay(100);
         }
       }
     }
    }
  
}
*/

