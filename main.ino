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


shape shapeDisplay(true);


void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(10);
  delay(1000);
}

void loop() 
{
  if (Serial.available()){
      shapeDisplay.decodeMsg();
  }
}

