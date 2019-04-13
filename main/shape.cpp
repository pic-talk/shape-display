/****************************************************************************
 * @project Shape Display API
 * @brief   making shapes with pic-talk-shape-display 
 * @detail  refer to http://pic-talk.org
 * @author  Musa ( http://musaunal.com )
 * @date    15-03-2019
 * @version 1.0.2

 History
 When           Who     What/Why
 -------------- ---     --------
 15-02-19      Musa     Create First module
 16-03-19      Musa     Stable Version
****************************************************************************/

#include "shape.h"
#include "constants.h"
#include <Wire.h>

// Set to true to print some debug messages, or false to disable them.
#define DEBUG_OUTPUT


char* lastAnimation = ":10000000000000000000000000000000000000000000000000000000000000000&";


//Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x42);
Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(0x44);
Adafruit_PWMServoDriver pwm4 = Adafruit_PWMServoDriver(0x48);

Adafruit_PWMServoDriver pwmDevices[4] = {pwm1,pwm2,pwm3,pwm4};


enum mode{SHAPEDIS,ADMIN, STATUS, CONTROL};

static const char *MODE_STRING[] = {
    "SHAPEDIS", "ADMIN", "STATUS", "CONTROL"
};

// const int maxBufferLength = 100;
// char inputRead[maxBufferLength];
// int chk = 0;



/****************************************************************************
 *                                   METHODS
 * ***************************************************************************/


shape::shape(bool msg0){

}

/****************************************************************************
 Function
    blink
 Parameters
    
 Returns
    None
 Description
    blink
 Info
  16.03.2019
****************************************************************************/

void shape::blink(){
  
  digitalWrite(13,1);
  delay(1000);
  digitalWrite(13,0);
  delay(1000);
}

/****************************************************************************
 Function
    getVersion
 Parameters
    None
 Returns
    Version of PIC-TALK API
 Description
   -
 Info
  16.03.2019
****************************************************************************/

int shape::getVersion(){
    return VERSION;
}

/****************************************************************************
 Function
    initialize
 Parameters
    None
 Returns
    None
 Description
    Should be called in setup for minimal requirements
 Info
  16.03.2019
****************************************************************************/

void shape::initialize(){
  
}


/****************************************************************************
 Function
    beginSerial
 Parameters
    Baud Rate = Manually 115200
 Returns
    None
 Description
    begin serial communication with computer
 Info
  16.03.2019
****************************************************************************/


void shape::beginSerial(int baudRate){
  Serial.begin(baudRate);
  Serial.println(":ax&");
  #ifdef DEBUG_OUTPUT
      Serial.print("Shape Display successfuly terminated. Version = ");
      Serial.println(VERSION);
  #endif

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

void shape::decodeMsg(char inputRead[], int chk){

  if(chk>0){

      delay(100);

      if(inputRead[0] == STARTBIT && inputRead[chk-1] == ENDBIT){
        
      #ifdef DEBUG_OUTPUT
         Serial.print("Incoming Message: ");
         Serial.println(inputRead);
         Serial.print("Mode: ");
         Serial.println(MODE_STRING[inputRead[1] -'0']);        //Printing Current Message Status
      #endif


        switch( inputRead[1]-'0'){

            case SHAPEDIS:                                     //The rest of the data should be height information about shape
                
              if(chk == DISPLAYSIZE + 3){                      // +3 for => STARTBIT, ENDBIT, MODE_STRING
                for(int i = 0; i < 4; i++)                     //Get number of PCA9685 module 
                {
                  pwmDevices[i].wakeupServo();
                  delay(200);
                  for(int l = 0; l < 16; l++)                  //Motor capacity per PCA9685 module 
                  {
                     pwmTurnMotor(pwmDevices[i],inputRead[2+i*16+l]-'0',l);
                     delay(50);

                     #ifdef DEBUG_OUTPUT
                        Serial.print(inputRead[2+i*16+l] -'0');
                        Serial.print(" ");
                     #endif
                  }
                  #ifdef DEBUG_OUTPUT
                     Serial.println();
                  #endif
                }
                
                delay(1000);
                for(size_t i = 0; i < 4; i++)
                {
                  pwmDevices[i].sleepServo();
                  delay(1000);
                }
                
                  
              }
              else{

                  #ifdef DEBUG_OUTPUT
                     Serial.println("The message is corrupted. Error");
                  #endif
              }
              break;
          
            case ADMIN:  
            {
            Serial.print("Yamuk");
            animation(inputRead,chk);
             
             }break;

            case STATUS:
                Serial.println(":ax/1.0.1/PT-BoardV1/&");
                break;
            
            case CONTROL:
               controlMotors();
               break;

            default:
              break;

        }
      }

      else{
         #ifdef DEBUG_OUTPUT
            Serial.print("The message is not in true format.");
            Serial.println(inputRead);
         #endif
      }

    }
}
/*
================================================================================
                                 PWM METHODS
================================================================================
*/


/****************************************************************************
 Function
    pwmInitialize
 Parameters
    Begin pwm devices
 Returns
    None
 Description
    PCA9685 can add with adresses to system
 Info
  18.03.2019
****************************************************************************/
void shape::pwmInitialize(int pwmFreq){

   pwm1.begin();
   pwm2.begin();
   pwm3.begin();
   pwm4.begin();
   delay(100);

   pwm1.setPWMFreq(pwmFreq);
   pwm2.setPWMFreq(pwmFreq);
   pwm3.setPWMFreq(pwmFreq);
   pwm4.setPWMFreq(pwmFreq);
   delay(100);

   pwm1.sleepServo();                  //Added in 20-03-2019 => It solved the power consumption issues
   pwm2.sleepServo();
   pwm3.sleepServo();
   pwm4.sleepServo();

   #ifdef DEBUG_OUTPUT
      Serial.println("PWM initialize completed....");
   #endif
}

/****************************************************************************
 Function
    pwmTurnMotor
 Parameters
   Adafruit_PWMServoDriver pwmDevice, int angle, int motorIndex, bool isDigitalServo
 Returns
    None
 Description
    Turn the motor with given angle
 Info
  18.03.2019
****************************************************************************/

void shape::pwmTurnMotor(Adafruit_PWMServoDriver pwmDevice, int angle, int motorIndex, bool isDigitalServo){

   
   int pulselength = map(angle, 0, 9, 102, 400);

   pwmDevice.setPWM(motorIndex,0,pulselength);
   
   /*
   #ifdef DEBUG_OUTPUT
     Serial.println(pulselength);
      Serial.print("Turning Motor: ");
      Serial.print(motorIndex);
      Serial.print("Angle: ");
      Serial.println(pulselength);
   #endif
*/
  
}


/****************************************************************************
 Function
    posZero
 Parameters
   
Returns
    None
 Description
    Set all motors to zero
 Info
  21.03.2019
****************************************************************************/

void shape::posZero(){

   for(int i = 0; i < 4; i++)     //Get number of PCA9685 module 
      {
      pwmDevices[i].wakeupServo();
      delay(10);
      for(int l = 0; l < 16; l++)               //Motor capacity per PCA9685 module 
      {
         pwmTurnMotor(pwmDevices[i],1,l);
       
      }
      pwmDevices[i].sleepServo();
      }

}

/****************************************************************************
 Function
    animation
Parameters
   
Returns
    None
 Description
   NOT TESTED
 Info
  28.03.2019
****************************************************************************/

void shape::animation(char animationBuffer[], int chk){

   Serial.print("Gelen : ");
   Serial.println(animationBuffer);
   int k = 0;
   pwmDevices[0].wakeupServo();
   for(int i = 2; i <67; i++){
      k = i-2;

      if(k%16==0){
            delay(100);
            if((k/16 != 0)){
               pwmDevices[(k/16)-1].sleepServo();
               Serial.print("Sleeped Device: ");
               Serial.print((k/16)-1);
            }
            if( k/16 != sizeof(pwmDevices)-1){
               pwmDevices[k/16].wakeupServo();
               Serial.print("Waked Device: ");
               Serial.println((k/16));
            }
         }

      if(animationBuffer[i] != lastAnimation[i]){
         
          
   
         Serial.print("PWM Device ID: ");
         Serial.print(k/16);
         Serial.print(" Motor ID: ");
         Serial.print(k%16);
         Serial.print(" Angle: ");
         Serial.println(animationBuffer[i]-'0');
         
        
         pwmTurnMotor(pwmDevices[k/16],animationBuffer[i]-'0',k%16);
         
         lastAnimation[i] = animationBuffer[i];
         delay(50);
      }
      else{
         lastAnimation[i] = animationBuffer[i];
      }

   }

   delay(100);
   pwmDevices[3].sleepServo();
   Serial.print("Sleeped Device: ");
   Serial.println("3");

   for(int t = 0; t<2; t ++){
      for(size_t i = 0; i < 4; i++)
      {  
         pwmDevices[i].wakeupServo();
         delay(50);
         pwmDevices[i].sleepServo();
         delay(50);
      }
   }
   Serial.print("Last Model: ");
   Serial.println(lastAnimation);

}


/****************************************************************************
 Function
    animation
Parameters
   
Returns
    None
 Description
   NOT TESTED
 Info
  28.03.2019
****************************************************************************/

void shape::controlMotors(){

   for(int i = 0; i < 4; i++)                     //Get number of PCA9685 module 
      {
         pwmDevices[i].wakeupServo();
         delay(200);
         for(int l = 0; l < 16; l++)                  //Motor capacity per PCA9685 module 
         {
            pwmTurnMotor(pwmDevices[i],9,l);
            delay(200);
            pwmTurnMotor(pwmDevices[i],0,l);
         }
      }
      
      delay(1000);
      for(size_t i = 0; i < 4; i++)
      {
         pwmDevices[i].sleepServo();
         delay(1000);
      }

}