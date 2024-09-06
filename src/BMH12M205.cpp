/*************************************************
File:       	  BMH12M205.cpp
Author:           BEST MODULES CORP.
Description:      UART communication with the BMH12M205
version:          V1.0.2-2024-8-28
**************************************************/
#include "BMH12M205.h"
BMH12M205_SET_t moduleSet;

/**********************************************************
  Description: Select the hardware serial port you need to use
  Parameters: *theSerial：hardware serial
               BMduino optional:serial(default) serial1/seria2/seria3/seria4
               UNO optional:serial(default)
  Return:      none
  Others:
**********************************************************/
BMH12M205::BMH12M205(HardwareSerial*theSerial)
{
  _serial = theSerial;
  _softSerial = NULL;
}
/**********************************************************
  Description: Select the software serial port RX TX you need to use
  Parameters:  rxPin:RX pin on the development board
               txPin:TX pin on the development board
  Return:      none
  Others:
**********************************************************/
BMH12M205::BMH12M205(uint8_t rxPin, uint8_t txPin)
{
  _serial = NULL;
  _rxPin = rxPin;
  _txPin = txPin;
  _softSerial = new SoftwareSerial(_rxPin, _txPin);
}
/*************************************************
Description:  Module Initial
Parameters:   baudRate
Return:       void
Others:
*************************************************/
void BMH12M205::begin(uint32_t baudRate)
{
  if (_softSerial != NULL)
  {
    _softSerial->begin(baudRate);
  }
  if (_serial != NULL)
  {
    _serial->begin(baudRate);
  }
}
/**********************************************************
Description: enter MODE_WEIGHT
Parameters:  none
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::wake()
{
  uint8_t readFlag = 1;
  uint8_t txBuf[11],rxBuf[5]={0};
  txBuf[0] = 0x55;
  txBuf[1] = 0x0B;
  txBuf[2] = 0xA0;
  txBuf[3] = MODE_WEIGHT;
  txBuf[4] = moduleSet.timer;
  if(txBuf[4]==0)
  {
    readSign=0;
  }
  else
  {
    readSign=1;
  }
  txBuf[5] = moduleSet.statusNotify;
  // txBuf[6] = lowByte(moduleSet.cal2Weight);
  // txBuf[7] = highByte(moduleSet.cal2Weight);
  // txBuf[8] = lowByte(moduleSet.cal3Weight);
  // txBuf[9] = highByte(moduleSet.cal3Weight);
  txBuf[10] = getChecksum(txBuf,10);
  writeBytes(txBuf, 11);
  delay(50);
  readFlag = readBytes(rxBuf,6,20);
  if (rxBuf[0] == 0xAA && readFlag == 0x00 && rxBuf[3] == 0x00)
    { 
      return 0;
    }
    else
    {
      return 1;
    }
}
/**********************************************************
Description: calibration Zero
Parameters:  none
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::calibrationZero()
{
  uint8_t readFlag = 1;
  uint8_t txBuf[11],rxBuf[5]={0};
  txBuf[0] = 0x55;
  txBuf[1] = 0x0B;
  txBuf[2] = 0xA0;
  txBuf[3] = MODE_TARE;
  txBuf[4] = moduleSet.timer;
  if(txBuf[4]==0)
  {
    readSign=0;
  }
  else
  {
    readSign=1;
  }
  txBuf[5] = moduleSet.statusNotify;
  // txBuf[6] = lowByte(moduleSet.cal2Weight);
  // txBuf[7] = highByte(moduleSet.cal2Weight);
  // txBuf[8] = lowByte(moduleSet.cal3Weight);
  // txBuf[9] = highByte(moduleSet.cal3Weight);
  txBuf[10] = getChecksum(txBuf,10);
  writeBytes(txBuf, 11);
  delay(100);
  readFlag = readBytes(rxBuf,6,20);
  if (rxBuf[0] == 0xAA && readFlag == 0x00 && rxBuf[3] == 0x00)
    { 
      delay(300);
      return 0;
    }
    else
    {
      delay(300);
      return 1;
    }
}
/**********************************************************
Description: set sleep mode
Parameters:  none
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::sleep()
{
  uint8_t readFlag = 1;
  uint8_t txBuf[11],rxBuf[5]={0};
  txBuf[0] = 0x55;
  txBuf[1] = 0x0B;
  txBuf[2] = 0xA0;
  txBuf[3] = MODE_SLEEP;
  txBuf[4] = moduleSet.timer;
  if(txBuf[4]==0)
  {
    readSign=0;
  }
  else
  {
    readSign=1;
  }
  txBuf[5] = 0x00;
  txBuf[6] = 0x00;
  txBuf[7] = 0x00;
  txBuf[8] = 0x00;
  txBuf[9] = 0x00;
  txBuf[10] = getChecksum(txBuf,10);
  writeBytes(txBuf, 11);
  delay(50);
  readFlag = readBytes(rxBuf,6,20);
  if (rxBuf[0] == 0xAA && readFlag == 0x00 && rxBuf[3] == 0x00)
    { 
      return 0;
    }
    else
    {
      return 1;
    }
}
/**********************************************************
Description: Obtain weight value, automatically output or actively request
Parameters:  none
Return:      WeightValue：Weight Value
Others:      none
**********************************************************/
int32_t BMH12M205::readWeight()
{
  int32_t WeightValue=0;
  BMH12M205_GET_t*get;
  uint8_t readFlag = 1;
  uint8_t rxBuf[14]={0};
  uint8_t txBuf[5]={0x55,0x05,0xA1,0x00,0X05};
  writeBytes(txBuf, 5);
  delay(50);
  readFlag = readBytes(rxBuf, 14,100);
  if (rxBuf[0] == 0xAA && readFlag == 0x00)
    {
       WeightValue= (int32_t)rxBuf[8] << 24 |(int32_t)rxBuf[7] << 16 |(int32_t)rxBuf[6] << 8 | (int32_t)rxBuf[5];
    }
  return WeightValue;
}
/**********************************************************
Description: Module sends instructions to retrieve data packets
Parameters:  *get 
                 bool filterStable;  //!< Filter stableflag
                 bool tareSuccess;   //!< Tare success flag
                 bool overLoad;      //!< OverLoad flag
                 int32_t weight;     //!< Weight
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::requestWeightInfo(BMH12M205_GET_t *get)
{
  uint8_t readFlag = 1;
  uint8_t rxBuf[14]={0};
  uint8_t txBuf[5]={0x55,0x05,0xA1,0x00,0X05};
  writeBytes(txBuf, 5);
  delay(50);
  readFlag = readBytes(rxBuf, 14,100);
  if (rxBuf[0] == 0xAA && readFlag == 0x00)
    {
      //get->status = rxBuf[3];
      get->filterStable = bitRead(rxBuf[4],0);
      get->tareSuccess = bitRead(rxBuf[4],1);
      get->overLoad = bitRead(rxBuf[4],2);
      get->weight = (int32_t)rxBuf[8] << 24 |(int32_t)rxBuf[7] << 16 |(int32_t)rxBuf[6] << 8 | (int32_t)rxBuf[5];
      //get->adc = rxBuf[12] << 24 |rxBuf[11] << 16 |rxBuf[10] << 8 | rxBuf[9];
      return 0;
    }
    else
    {
      return 1;
    }
}
/**********************************************************
Description: Determine if the data packet is automatically output by the module
Parameters:  none   
Return:      true:It is a data packet automatically output by the module
             false:Data packets that are not automatically output by the module
Others:      none
**********************************************************/
bool BMH12M205::isInfoAvailable()
{
  clear_BUFF();
  uint8_t header[14];
  uint8_t recBuf[14];
  header[0]=0xAA;
  header[1]=0x0E;
  header[2]=0xA1;
  uint8_t i, num = 0, readCnt = 0, failCnt = 0, checkCode = 0;
  bool isHeader = 0, result = false;

  /* Select hardSerial or softSerial according to the setting */
  if (_softSerial != NULL)
  {
    num = _softSerial->available();
  }
  else if (_serial != NULL)
  {
    num = _serial->available();
  }
  /* Serial buffer contains at least one 32-byte data */
  if (num >= 0x0E)
  {
    while (failCnt < 2) // Didn't read the required data twice, exiting the loop
    {
      /* Find 5-byte data header */
      for (i = 0; i < 2;)
      {
        if (_softSerial != NULL)
        {
          recBuf[i] = _softSerial->read();
        }
        else if (_serial != NULL)
        {
          recBuf[i] = _serial->read();
          
        }
        if (recBuf[i] == header[i])
        {
          isHeader = 1; // Fixed code is correct
          i++;             // Next byte
        }
        else if (recBuf[i] != header[i] && i > 0)
        {
          isHeader = 0; // Next fixed code error
          failCnt++;
          break;
        }
        else if (recBuf[i] != header[i] && i == 0)
        {
          readCnt++; // 0xAA not found, continue
        }
        if (readCnt >= (num - 5))
        {
          readCnt = 0;
          isHeader = 0; //
          break;
        }
      }
      /* Find the correct data header */
      if (isHeader)
      {
        recBuf[0]==0xAA;
        recBuf[1]==0x0E;
        for (i = 0; i < 0x0c; i++) // Read subsequent 27-byte data
        {
          if (_softSerial != NULL)
          {
            recBuf[i+2] = _softSerial->read();
          }
          else if (_serial != NULL)
          {
            recBuf[i+2] = _serial->read();
          }
          checkCode += recBuf[i]; // Sum checkCode
        }
        checkCode = ~checkCode + 1; // Calculate checkCode
        /* Compare whether the check code is correct */
        if (checkCode == recBuf[0x0e - 1])
        {
          for (i = 0; i < 0x0e; i++) // True, assign data to _recBuf[]
          {
            _recBuf[i] = recBuf[i];
          }
          result = true;
          break; // Exit "while (failCnt < 2)" loop
        }
        else
        {
          failCnt++; // Error, failCnt plus 1, return "while (failCnt < 2)" loop
          checkCode = 0;
        }
      }
    }
    clear_UART_FIFO();
  }
  return result;
}
/**********************************************************
Description: Read automatically output packets
Parameters:  *get 
                bool filterStable;  //!< Filter stableflag
                bool tareSuccess;   //!< Tare success flag
                bool overLoad;      //!< OverLoad flag
                int32_t weight;     //!< Weight  
Return:      void
Others:      none
**********************************************************/
void BMH12M205::readWeightInfo(BMH12M205_GET_t *get)
{
     // get->status = _recBuf[3];
      get->filterStable = bitRead(_recBuf[4],0);
      get->tareSuccess = bitRead(_recBuf[4],1);
      get->overLoad = bitRead(_recBuf[4],2);
      get->weight =(int32_t)_recBuf[8] << 24 |(int32_t)_recBuf[7] << 16 |(int32_t)_recBuf[6] << 8 | (int32_t)_recBuf[5];
     // get->adc =_recBuf[12] << 24 |_recBuf[11] << 16 |_recBuf[10] << 8 | _recBuf[9];
}
/**********************************************************
Description: Read automatically output weight
Parameters:  none  
Return:      WeightValue：Weight Value
Others:      none
**********************************************************/
int32_t BMH12M205::readWeightInfo()
{
      int32_t WeightValue=0;
      WeightValue =(int32_t)_recBuf[8] << 24 |(int32_t)_recBuf[7] << 16 |(int32_t)_recBuf[6] << 8 | (int32_t)_recBuf[5];
      return WeightValue;
}
/**********************************************************
Description: get module Infomation
Parameters:  *info
                uint16_t softwareVer;   //!< Software version
                uint16_t protocolVer;   //!< Protocol version
                uint32_t moduleID;      //!< Module ID   
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::getModuleInfo(BMH12M205_INFO_t *info)
{
  uint8_t readFlag = 1;
  uint8_t txBuf[5]={0x55,0x05,0xF1,0x00,0xB5};
  uint8_t rxBuf[12]={0};
  writeBytes(txBuf, 5);
  delay(50);
  readFlag = readBytes(rxBuf, 12,100);
  if (rxBuf[0] == 0xAA && readFlag == 0x00)
    {
      info->softwareVer = rxBuf[4] << 8 | rxBuf[3];
      info->protocolVer = rxBuf[6] << 8 | rxBuf[5];
      info->moduleID = (int32_t)rxBuf[10] << 24 |(int32_t)rxBuf[9] << 16 |(int32_t)rxBuf[8] << 8 | (int32_t)rxBuf[7];
      return 0;
    }
    else
    {
      return 1;
    }  
}
/**********************************************************
Description: read Parameter
Parameters:  address:Parameter address
             uint8_t buf[]:data
             lenght:write length 
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::getParameter(uint8_t address, uint8_t buf[],uint8_t lenght)
{
  uint8_t readFlag = 1;
  uint8_t txBuf[6]={0x55,0x06,0xB1,address,lenght};
  uint8_t rxBuf[lenght+5]={0};
  txBuf[5] = getChecksum(txBuf,5);
  writeBytes(txBuf, 6);
  delay(50);
  readFlag = readBytes(rxBuf, lenght + 5,100);
  if (rxBuf[0] == 0xAA && readFlag == 0x00)
    {
      for (size_t i = 0; i < lenght; i++)
      {
        buf[i] = rxBuf[i+4];
      }
      return 0;
    }
    else
    {
      return 1;
    }
}
/**********************************************************
Description: set weight uint
Parameters:  0:g
             1:0.1g
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::setWeighingUnit(uint8_t unit)
{
  uint8_t buff[1];
  buff[0]=unit;
  return(setParameter(ADD_UINT_WEIGHT,buff,1));
}
/**********************************************************
Description: set InfoNotify Status
Parameters:  timer:Interval time  =50ms*timer
             statusNotify: true/false
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::setInfoNotifyStatus(uint8_t timer,bool statusNotify)
{
  uint8_t readFlag = 1;
  uint8_t txBuf[11],rxBuf[5]={0};
  moduleSet.timer=timer;
  moduleSet.statusNotify=statusNotify;
  txBuf[0] = 0x55;
  txBuf[1] = 0x0B;
  txBuf[2] = 0xA0;
  txBuf[3] = 0x00;
  txBuf[4] = moduleSet.timer;
  if(txBuf[4]==0)
  {
    readSign=0;
  }
  else
  {
    readSign=1;
  }
  txBuf[5] = moduleSet.statusNotify;
  // txBuf[6] = lowByte(moduleSet.cal2Weight);
  // txBuf[7] = highByte(moduleSet.cal2Weight);
  // txBuf[8] = lowByte(moduleSet.cal3Weight);
  // txBuf[9] = highByte(moduleSet.cal3Weight);
  txBuf[10] = getChecksum(txBuf,10);
  writeBytes(txBuf, 11);
  delay(50);
  readFlag = readBytes(rxBuf, 6,20);
  if (rxBuf[0] == 0xAA && readFlag == 0x00 && rxBuf[3] == 0x00)
    { 
      return 0;
    }
    else
    {
      return 1;
    }
}
/**********************************************************
Description: write Parameter
Parameters:  address:Parameter address
             uint8_t buf[]:data
             lenght:write length
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::setParameter(uint8_t address, uint8_t buf[],uint8_t lenght)
{
  uint8_t readFlag = 1;
  uint8_t rxBuf[6]={0};
  uint8_t txBuf[lenght + 5];
  txBuf[0] = 0x55;
  txBuf[1] = lenght + 5;
  txBuf[2] = 0xB0;
  txBuf[3] = address;
  for (size_t i = 0; i < lenght; i++)
  {
    txBuf[i + 4] = buf[i];
  }
  txBuf[lenght + 4] = getChecksum(txBuf,lenght + 4);
  writeBytes(txBuf, lenght + 5);
  delay(50);
  readFlag = readBytes(rxBuf, 6,50);
  if (rxBuf[0] == 0xAA && readFlag == 0x00 && rxBuf[3] == 0x00)
    {
      return 0;
    }
    else
    {
      return rxBuf[3];
    }
}
/**********************************************************
Description: set Work Mode
Parameters:  *setdata 
                  uint8_t wordMode;       //!<@see Work mode config
                  uint8_t timer;          //!< Timed Send 0xA1 Answer
                  bool statusNotify;      //!< Status Notify
                  uint16_t cal2Weight;    //!< Calibration Point 2 Weight
                  uint16_t cal3Weight;    //!< Calibration Point 3 Weight
Return:      0：Success
             1：Failure
Others:      none
**********************************************************/
uint8_t BMH12M205::setWorkMode(BMH12M205_SET_t *setdata)
{
  uint8_t readFlag = 1;
  uint8_t txBuf[11],rxBuf[5]={0};
  txBuf[0] = 0x55;
  txBuf[1] = 0x0B;
  txBuf[2] = 0xA0;
  txBuf[3] = setdata->wordMode;
  txBuf[4] = setdata->timer;
    if(txBuf[4]==0)
  {
    readSign=0;
  }
  else
  {
    readSign=1;
  }
  txBuf[5] = setdata->statusNotify;
  txBuf[6] = lowByte(setdata->cal2Weight);
  txBuf[7] = highByte(setdata->cal2Weight);
  txBuf[8] = lowByte(setdata->cal3Weight);
  txBuf[9] = highByte(setdata->cal3Weight);
  txBuf[10] = getChecksum(txBuf,10);
  writeBytes(txBuf, 11);
  delay(50);
  readFlag = readBytes(rxBuf,5,20);
  if (rxBuf[0] == 0xAA && readFlag == 0x00 && rxBuf[3] == 0x00)
    { 
      return 0;
    }
    else
    {
      return 1;
    }
}
/**********************************************************
Description: Write data through UART
Parameters: wbuf:The array for storing Data to be sent
            wlen:Length of data sent
Return: None
Others: None
**********************************************************/
void BMH12M205::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
  if (_softSerial != NULL)
  {
    while (_softSerial->available() > 0)
    {
      _softSerial->read();
    }
    _softSerial->write(wbuf, wlen);
  }
  /* Select HardwareSerial Interface */
  if (_serial != NULL)
  {
    while (_serial->available() > 0)
    {
      _serial->read();
    }
    _serial->write(wbuf, wlen); 
  }
}
/**********************************************************
Description: Read data through UART
Parameters: rbuf: Used to store received data
            rlen: Length of data to be read
            timeout:Module response timeout
Return:  0:CHECK_OK
         1:CHECK_ERROR
         2:TIMEOUT_ERROR
Others: None
**********************************************************/
uint8_t BMH12M205::readBytes(uint8_t rbuf[], uint8_t rlen,uint8_t timeout)
{
  uint8_t i = 0, delayCnt = 0,checkSum=0;
/* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_softSerial->available() == 0)
      {
        if (delayCnt > timeout)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _softSerial->read();
    }
  }
/* Select HardwareSerial Interface */
  else
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_serial->available() == 0)
      {
        if (delayCnt > timeout)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _serial->read();
    }
  }
  /* check Sum */
  for (i = 0; i < (rlen - 1); i++)
  {
    checkSum += rbuf[i];
  }
  checkSum = ~checkSum + 1;
  if (checkSum == rbuf[rlen - 1])
  {
    return CHECK_OK; // Check correct
  }
  else
  {
    return CHECK_ERROR; // Check error
  }
}
/**********************************************************
Description: Obtain the verification code formed by the array
Parameters:  Buf[]:txBuf:6-byte array to be sent
             lenght：buff length
Return: checksum:Equal to the sum of data of other bytes
Others: None
**********************************************************/
uint8_t BMH12M205::getChecksum(uint8_t Buf[],uint8_t lenght)
{
  uint8_t checksum = 0;
  checksum = 0;
  for (uint8_t i = 0; i < lenght; i++)
  {
    checksum += Buf[i];
  }
  checksum = ~checksum + 1;
  return checksum;
}
/**********************************************************
Description: eliminate buff data
Parameters:  none
Return:      void    
Others:      
**********************************************************/ 
void BMH12M205::clear_UART_FIFO()
{
    if (_softSerial != NULL)
  {
    while (_softSerial->available() > 0)
    {
      _softSerial->read();
    }
  }
  else
  {
    while (_serial->available() > 0)
    {
      _serial->read();
    }
  }
}
/**********************************************************
Description: eliminate buff data
Parameters:  none
Return:      void    
Others:      
**********************************************************/ 
void BMH12M205::clear_BUFF()
{
  for(int i=0;i<14;i++)
  {
    _recBuf[i]=0;
  }
}
