/*-----------------------------------------------*
 * Exapmle Sketch for OPENTAC from Musa Sadık    *                                          
 * Unal                                          *
 * 15.07.2018                                    * 
 *-----------------------------------------------*/


#include <Wire.h>
#include "shape.h"

#define STARTBIT 0x3A // :
#define ENDBIT   0x26 // &

#define DISPLAYSIZE 64

enum mode{SHAPEDIS,ADMIN};

static const char *MODE_STRING[] = {
    "SHAPEDIS", "ADMIN"
};

const int maxBufferLength = 100;
char inputRead[maxBufferLength];
int chk = 0;





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
    
    chk = Serial.readBytes(inputRead,maxBufferLength);

    decodeMsg(inputRead[maxBufferLength]);
  }
}

void decodeMsg(byte message[maxBufferLength] ){
  
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