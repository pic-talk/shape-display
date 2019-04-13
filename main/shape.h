#ifndef t1
#define t1

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include <WProgram.h>
#endif


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


class shape{

    public:
        shape(bool msg0 = false);
        
        
        //Methods
        void pwmInitialize(int pwmFreq = 50);
        void pwmTurnMotor(Adafruit_PWMServoDriver pwmDevice, int angle, int motorIndex, bool isDigitalServo = true);
        
        void decodeMsg(char inputRead[],int chk);
        void animation(char animationBuffer[], int chk);
        void beginSerial(int baudRate = 115200); //Default set as 115200   
        void initialize();
        void posZero();
        void controlMotors();

        void blink();
        int getVersion();

    private:
        const int maxBufferLength = 100;
        

};

#endif 
