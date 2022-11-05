#pragma once
#include "Arduino.h"
#include "flprogModbusUtilites.h"

class ModbusBridgeTCPDevice
{
public:
    virtual void begin(bool mode){};
    virtual byte available() { return 0; };
    virtual byte read() { return 0; };
    virtual bool connected() { return false; };
    virtual void stop(){};
    virtual void connect(bool mode){};
    virtual byte write(byte buffer[], byte size) { return 0; };
    bool setPort(int port);
    virtual void restartServer(bool mode){};
    bool setRemoteIp(byte ipFirst, byte ipSecond, byte ipThird, byte ipFourth);

protected:
    int tcpPort = 502;
    byte ipFirst;
    byte ipSecond;
    byte ipThird;
    byte ipFourth;

private:
    bool compareRemoteIp(byte ipFirst, byte ipSecond, byte ipThird, byte ipFourth);
};

class ModbusBridgeRTUDevice
{
public:
    virtual byte available() { return 0; };
    virtual byte read() { return 0; };
    virtual byte write(byte buffer[], byte size) { return 0; };
    virtual void begin(){};
    long t35TimeForSpeed();
    long timeForSendbytes(byte bufferSize);
    void setPortSpeed(byte speed);
    void setPortDataBits(byte dataBits);
    void setPortStopBits(byte stopBits);
    void setPortParity(byte stopBits);

protected:
    virtual bool hasPort() { return false; };
    virtual void restartPort(){};
    byte portSpeed = SPEED_9600;
    byte portDataBits = 8;
    byte portParity = 0;
    byte portStopBits = 1;

private:
};

class ModbusBridge
{
public:
    void pool();
    void setTCPDevice(ModbusBridgeTCPDevice *device);
    void setRTUDevice(ModbusBridgeRTUDevice *device);
    void setTCPPort(int port);
    void setTCPRemoteIp(byte ipFirst, byte ipSecond, byte ipThird, byte ipFourth);
    void setRtuPortSpeed(byte speed);
    void setRtuPortDataBits(byte dataBits);
    void setRtuPortStopBits(byte stopBits);
    void setRtuPortParity(byte stopBits);
    void byServer();
    void byClient();
    void begin();

protected:
private:
    void tcpPool();
    void rtuPool();
    void sendRTUBuffer();
    void getRTURxBuffer();
    void sendTCPBuffer();
    void onPeDePin();
    void offPeDePin();
    void setPinPeDe(byte pin);
    byte pinPeDe = 200;
    byte bufferSize = 0;
    byte mbapBuffer[6];
    byte buffer[64];
    byte lastRec = 0;
    byte workStatus = MODBUS_READY;
    unsigned long startT35;
    ModbusBridgeTCPDevice *tcpDevice;
    ModbusBridgeRTUDevice *rtuDevice;
    unsigned long startSendTime;
    int timeOfSend;
    bool isServer = true;
};
