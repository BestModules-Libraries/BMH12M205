/*************************************************
File:       	BMH12M105.h
Author:         BEST MODULES CORP.
Description:    Define classes and required variables
version:        V1.0.2-2024-8-28
**************************************************/
#ifndef _BMH12M205_H__
#define _BMH12M205_H__

#include <Arduino.h>
#include <SoftwareSerial.h>
#define CHECK_OK 0
#define CHECK_ERROR 1
#define TIMEOUT_ERROR 2
typedef struct {
    uint8_t wordMode;       //!<@see Work mode config
    uint8_t timer;          //!< Timed Send 0xA1 Answer
    bool statusNotify;      //!< Status Notify
    uint16_t cal2Weight;    //!< Calibration Point 2 Weight
    uint16_t cal3Weight;    //!< Calibration Point 3 Weight
} BMH12M205_SET_t;
typedef struct {
    //uint8_t status;     //!< Work status
    bool filterStable;  //!< Filter stableflag
    bool tareSuccess;   //!< Tare success flag
    bool overLoad;      //!< OverLoad flag
    int32_t weight;     //!< Weight
   // int32_t adc;        //!< Adc data
} BMH12M205_GET_t;
typedef struct {
    uint16_t softwareVer;   //!< Software version
    uint16_t protocolVer;   //!< Protocol version
    uint32_t moduleID;      //!< Module ID
} BMH12M205_INFO_t;

class BMH12M205
{
public:
     BMH12M205(HardwareSerial *theSerial = &Serial);
     BMH12M205(uint8_t rxPin, uint8_t txPin);
     void begin(uint32_t baudRate = 38400);
     uint8_t wake();
     uint8_t calibrationZero();
     uint8_t sleep();
     int32_t readWeight();
     uint8_t requestWeightInfo(BMH12M205_GET_t *get);
     bool isInfoAvailable();
     void  readWeightInfo(BMH12M205_GET_t *get);
     uint8_t getModuleInfo(BMH12M205_INFO_t *info);
     uint8_t getParameter(uint8_t address, uint8_t buf[],uint8_t lenght);
     uint8_t setWeighingUnit(uint8_t unit);
     uint8_t setInfoNotifyStatus(uint8_t timer,bool statusNotify);
     uint8_t setParameter(uint8_t address, uint8_t buf[],uint8_t lenght);

private:
     uint8_t _rxPin;
     uint8_t _txPin;
     uint8_t readSign=0;     
     HardwareSerial *_serial = NULL;
     SoftwareSerial *_softSerial = NULL;
     uint8_t _recBuf[14] = {0}; // Array for storing received data
     int32_t readWeightInfo();
     uint8_t setWorkMode(BMH12M205_SET_t *setdata);
     void writeBytes(uint8_t wbuf[], uint8_t wlen);
     uint8_t readBytes(uint8_t rbuf[], uint8_t rlen,uint8_t timeout);
     uint8_t getChecksum(uint8_t Buf[],uint8_t lenght);
     void clear_BUFF();
     void clear_UART_FIFO();
};
/*! Work mode config */
#define MODE_WEIGHT      0x00
#define MODE_CALIBRATE   0x01
#define MODE_TARE        0x02
#define MODE_SLEEP       0x03
/*! paragrameter address */
#define ADD_TIMER_WAKEUP      0x80
#define ADD_SAMPLE_WAKEUP     0x81
#define ADD_UPWEIGHT_WAKEUP   0x82
#define ADD_DOMWEIGHT_WAKEUP  0x84
#define ADD_SAMPLE_WEIGHT     0x86
#define ADD_CREEP_FIX         0x87
#define ADD_TARE_POWERON      0x88
#define ADD_BRUD_RATE         0x89
#define ADD_FLITER_LEVER      0x8A
#define ADD_STABLE_VALUE      0x8B
#define ADD_UINT_WEIGHT       0x8C
/*! sample rate */
#define RATE_4HZ    0x00
#define RATE_10HZ   0x02
#define RATE_41HZ   0x06
#define RATE_163HZ  0x0A
#define RATE_326HZ  0x0C
#define RATE_1302HZ 0x0F
/*! weight uint */
#define UINT_G    0x00
#define UINT_KG   0x01
/*! sample rate */
#define BAUD_2400   0x00
#define BAUD_4800   0x01
#define BAUD_9600   0x02
#define BAUD_14400  0x03
#define BAUD_19200  0x04
#define BAUD_28800  0x05
#define BAUD_38400  0x06
#define BAUD_56000  0x07
#define BAUD_57600  0x08
#define BAUD_115200 0x09

#endif
