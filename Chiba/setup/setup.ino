#include <Adafruit_NeoPixel.h>
#define PIN        9
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
