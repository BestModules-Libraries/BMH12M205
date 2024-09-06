/*****************************************************************
File:         readWeight_AutoOutput.ino
Description:  Enable automatic output, host scans data packets, and outputs weight values.
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
   weight.setInfoNotifyStatus(2,false);//Module auto output WeightInfo,Interval time = 100ms
   weight.calibrationZero();//need 300ms
}
void loop()
{
  if(weight.isInfoAvailable()==true)
  {
    weight.readWeightInfo(getData);
    Serial.print(getData.weight,DEC);
    Serial.println("g");   //Parameter uint default g
  }
}
