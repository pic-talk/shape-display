/****************************************************************************
 Module
   main.ino
 Revision
   1.0.1
 Description  
    
 History
 When             Who     What/Why
 16.03.2019       musa    first initiliaze
****************************************************************************/

#include <Wire.h>
#include "shape.h"
#include <Adafruit_PWMServoDriver.h>

#include <SoftwareSerial.h>

SoftwareSerial bt(12,11);

shape shapeDisplay(true);
char inputRead[100];



int chk = 0;

void setup()
{
  bt.begin(9600);
  bt.setTimeout(10);
  
  shapeDisplay.pwmInitialize();
  delay(50);
  Serial.begin(115200);
  Serial.setTimeout(10);
  delay(1000);
  //shapeDisplay.posZero();
  
} 

void loop()
{
  
  if (Serial.available())
  {
   chk = Serial.readBytes(inputRead,100);
   shapeDisplay.decodeMsg(inputRead,chk);
   
     //sadeceprintln olan satır mı gidiyor? evet şu an
     //bunu sadece birkez göndertebilir miyiz? olur mu?
  }

}
