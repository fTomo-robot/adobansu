#include <Arduino.h>
#include <Wire.h>

#define addr 0x70//PCA9547Dのアドレス
#define ADDRESS 0x52//ToFのアドレス

#define WIRE1_SDA       6  // Use GP6 as I2C1 SDA
#define WIRE1_SCL       7  // Use GP7 as I2C1 SCL
arduino::MbedI2C Wire1(WIRE1_SDA, WIRE1_SCL);

  // 閾値
  int threshold_F = 100;
  int threshold_R = 100;
  int threshold_L = 100;

int val_0;
int val_1;
int val_2;
int val_3;
/*
val_0 = 前右
val_1 = 横右
val_2 = 横左
val_3 = 前左
*/

int val_Front;

uint16_t distance;
uint16_t distance_tmp;
uint8_t data_cnt;


void ChangeChannel(char channel) //バスマルチのチャンネル変更
{
  Wire1.beginTransmission(addr);
  Wire1.write(channel & 0x07 | 0x08);
  Wire1.endTransmission();
}

void ToF()
{
    //Serial.print("distanceF1 = ");

    Wire1.beginTransmission(ADDRESS);
    Wire1.write(0xD3);
    Wire1.endTransmission(false);
    Wire1.requestFrom(ADDRESS, 2);
    data_cnt = 0;
    distance = 0;
    distance_tmp = 0;
    while (Wire1.available()) {
      distance_tmp = Wire1.read();
      distance = (distance << (data_cnt * 8)) | distance_tmp;
      data_cnt++;
    }

    //Serial.print(distance);
    //Serial.println(" mm");
}


void setup() {
 Wire1.begin();
 Serial.begin(9600);
 Serial1.begin(9600);
 Serial.println("MTOF171000C0_I2C");
}

void loop() {
  //ToFセンサ読み取り
{  
 ChangeChannel(0);
 ToF();
 val_0 = distance;
 Serial.print("ToF_0 = ");
 Serial.print(val_0);
 Serial.println(" mm");
 delay(5);
}
{  
 ChangeChannel(1);
 ToF();
 val_1 = distance;
 Serial.print("ToF_1 = ");
 Serial.print(val_1);
 Serial.println(" mm");
  delay(5);
}
{  
 ChangeChannel(2);
 ToF();
 val_2 = distance;
 Serial.print("ToF_2 = ");
 Serial.print(val_2);
 Serial.println(" mm");
  delay(5);
}
{  
 ChangeChannel(3);
 ToF();
 val_3 = distance;
 Serial.print("ToF_3 = ");
 Serial.print(val_3);
 Serial.println(" mm");
  delay(5);
}
  //前センサ比較

  int val_Front = min(val_0, val_3);
  Serial.print("小さい方の値: ");
  Serial.println(val_Front);


  int result_0 = (val_Front < threshold_F) ? 1 : 0;
  int result_1 = (val_1 < threshold_R) ? 1 : 0;
  int result_2 = (val_2 < threshold_L) ? 1 : 0;

  // 結果を出力(パソコン)
  {
  Serial.print("[");
  Serial.print(result_0);
  Serial.print(",");
  Serial.print(result_1);
  Serial.print(",");
  Serial.print(result_2);
  Serial.println("]");
  }
  // 結果を出力(メインマイコン)
  {
  Serial1.print(result_0);
  Serial1.print(",");
  Serial1.print(result_1);
  Serial1.print(",");
  Serial1.println(result_2);
  }

}