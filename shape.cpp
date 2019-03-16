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
