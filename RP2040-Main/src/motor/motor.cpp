#include "./motor.h"

MOTOR::MOTOR(HardwareSerial* ptr_serial, SMS_STS* ptr_st){
    SER = ptr_serial;
    ST = ptr_st;
}

void MOTOR::init(void){
    SER->begin(1000000);
    ST->pSerial = SER;
    delay(1000);
    for(int i=1; i<5; i++){
        ST->unLockEprom(i);
        ST->WheelMode(i);//恒速模式
        ST->LockEprom(i);
    }
}

void MOTOR::forward(int speed, int accer){
    ST->WriteSpe(1,speed,accer);
    ST->WriteSpe(2,speed,accer);
    ST->WriteSpe(3,-1*speed,accer);
    ST->WriteSpe(4,-1*speed,accer);
}

void MOTOR::right(int speed, int accer){
    ST->WriteSpe(1,speed,accer);
    ST->WriteSpe(2,speed,accer);
    ST->WriteSpe(3,speed,accer);
    ST->WriteSpe(4,speed,accer);
}

void MOTOR::left(int speed, int accer){
    speed = -1 * speed;
    ST->WriteSpe(1,speed,accer);
    ST->WriteSpe(2,speed,accer);
    ST->WriteSpe(3,speed,accer);
    ST->WriteSpe(4,speed,accer);
}

void MOTOR::stop(void){
    ST->WriteSpe(1,0,100);
    ST->WriteSpe(2,0,100);
    ST->WriteSpe(3,0,100);
    ST->WriteSpe(4,0,100);
}


//   st.WriteSpe(1, 3400, 50);//舵机(ID1)以最高速度V=3400步/秒，加速度A=50(50*100步/秒^2)，旋转
//   delay(4000);
//   st.WriteSpe(1, 0, 50);//舵机(ID1)以加速度A=50(50*100步/秒^2)，停止旋转(V=0)
//   delay(2000);
//   st.WriteSpe(1, -3400, 50);//舵机(ID1)以最高速度V=-3400步/秒，加速度A=50(50*100步/秒^2)，反向旋转
//   delay(4000);
//   st.WriteSpe(1, 0, 50);//舵机(ID1)以加速度A=50(50*100步/秒^2)，停止旋转(V=0)
//   delay(2000);