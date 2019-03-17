/****************************************************************************
 Module
   shape.cpp
 Revision
   1.0.0
 Description
   
 
 History
 When           Who     What/Why
 -------------- ---     --------
 15/03/19    
****************************************************************************/

#include "shape.h"
#include "constants.h"


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwmDevices[4] ={};

enum mode{SHAPEDIS,ADMIN};

static const char *MODE_STRING[] = {
    "SHAPEDIS", "ADMIN"
};

const int maxBufferLength = 100;
char inputRead[maxBufferLength];
int chk = 0;



shape::shape(bool msg0){

}


void shape::blink(){
  
  digitalWrite(13,1);
  delay(1000);
  digitalWrite(13,0);
  delay(1000);
}


int shape::getVersion(){
    return VERSION;
}



void shape::begin(int baudRate){

    pinMode(13,OUTPUT);
    Serial.begin(baudRate);
    Serial.print("Shape Display successfuly terminated. Version = ");
    Serial.println(VERSION);

}


/****************************************************************************
 Function
    addPwmDevices
 Parameters
    Adresses of PCA9685 device
 Returns
    None
 Description
    PCA9685 can add with adresses to system
 Info
  16.03.2019
****************************************************************************/

void shape::addPwmDevices(byte adress1, byte adress2, byte adress3, byte adress4){

    Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(adress1);
    Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(adress2);
    Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(adress3);
    Adafruit_PWMServoDriver pwm4 = Adafruit_PWMServoDriver(adress4);
    
    pwmDevices[0] = pwm1;
    pwmDevices[1] = pwm2;
    pwmDevices[2] = pwm3;
    pwmDevices[3] = pwm4;
}

/****************************************************************************
 Function
    decodeMsg
 Parameters
    None
 Returns
    None
 Description
    Read from serial and decode the 
 Info 
  16.03.2019
****************************************************************************/

void shape::decodeMsg(){
  
  chk = Serial.readBytes(inputRead,maxBufferLength);

  if(chk>0){

      if(inputRead[0] == STARTBIT && inputRead[chk-1] == ENDBIT){
        
        Serial.print("Incoming Message: ");
        Serial.println(inputRead);
        Serial.print("Mode: ");
        Serial.println(MODE_STRING[inputRead[1] -'0']);  //Printing Current Message Status

        switch( inputRead[1]-'0'){

            case SHAPEDIS:                  //The rest of the data should be height information about shape
                
              if(chk == DISPLAYSIZE + 3){  // +3 for => STARTBIT, ENDBIT, MODE_STRING
                for(size_t i = 0; i < DISPLAYSIZE/16; ++i) //Get number of PCA9685 module 
                {
                  for(size_t l = 0; l < 16; l++)  //Motor capacity per PCA9685 module 
                  {
                      Serial.print(inputRead[2+i*16+l]);
                      Serial.print(" ");
                  }
                  Serial.println();
                }
              }
              else{
                Serial.println("The message is corrupted. Error");
              }
              break;
          
            case ADMIN:

              Serial.println("Doğru");
              break;

            default:
              break;

        }
      }

      else{
        Serial.print("The message is not in true format.");
        Serial.println(inputRead);
      }

    }
}

