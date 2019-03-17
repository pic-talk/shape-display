#ifndef t1
#define t1

#if (ARDUINO >= 100)
    #include "Arduino.h"
#endif

class shape{

    public:
        shape(bool msg0 = false);
        
        //Methods
        void addPwmDevices(byte adress1, byte adress2, byte adress3, byte adress4);
        void blink();
        void decodeMsg();
        void begin(int baudRate = 115200); //Default set as 115200   
        int getVersion();

    private:


};

#endif 
