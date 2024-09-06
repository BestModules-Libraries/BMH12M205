/*****************************************************************
File:         readWeight.ino
Description:  Turn off automatic output, and the host sends CMD to obtain the weight value.
Note:
******************************************************************/
#include "BMH12M205.h"
BMH12M205 weight(6,7);        // Please uncomment out this line of code if you use softSerial on BMduino
//BMH12M205 weight(&Serial1); // Please uncomment out this line of code if you use Serial1 on BMduino
//BMH12M205 weight(&Serial2); // Please uncomment out this line of code if you use Serial2 on BMduino
BMH12M205_GET_t getData;
void setup()
{
   Serial.begin(38400); // start serial for output
   weight.begin();
   weight.setInfoNotifyStatus(0,false);//Close Module auto output WeightInfo
   weight.calibrationZero();//need 300ms
}
void loop()
{
    Serial.print(weight.readWeight(),DEC);
    Serial.println("g");   //Parameter uint default g
}
