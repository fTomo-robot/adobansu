// made by Tomoshibi @Jun_robot
#ifndef _H_MOTOR_
#define _H_MOTOR_

#include <Arduino.h>
#include <SCServo.h>
#include <HardwareSerial.h>

class MOTOR{
    private:
        HardwareSerial* SER;
        SMS_STS* ST;
        long timer;
    
    public:
        MOTOR(HardwareSerial* ptr_serial, SMS_STS* ptr_st);

        void init();
        void stop(void);

        void forward(int speed, int accer);
        void left(int speed, int accer);
        void right(int speed, int accer);
};

#endif