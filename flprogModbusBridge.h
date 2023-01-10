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
    virtual void print(String data);

protected:
    int tcpPort = 502;
    byte ipFirst;
    byte ipSecond;
    byte ipThird;
    byte ipFourth;

private:
    bool compareRemoteIp(byte ipFirst, byte ipSecond, byte ipThird, byte ipFourth);
};

class ModbusBridge
{
public:
    virtual void pool();
    void setTCPDevice(ModbusBridgeTCPDevice *device);
    void setRTUDevice(FLProgUart *device);
    void setTCPPort(int port);
    void setTCPRemoteIp(byte ipFirst, byte ipSecond, byte ipThird, byte ipFourth);
    void setRtuPortSpeed(byte speed);
    byte getRtuPortSpeed() { return rtuDevice->getPortSpeed(); };
    void setRtuPortDataBits(byte dataBits);
    byte getRtuPortDataBits() { return rtuDevice->getPortDataBits(); };
    void setRtuPortStopBits(byte stopBits);
    byte getRtuPortStopBits() { return rtuDevice->getPortStopBits(); };
    void setRtuPortParity(byte stopBits);
    byte getRtuPortParity() { return rtuDevice->getPortParity(); };
    void setPinPeDe(byte pin);
    void byServer();
    void byClient();
    virtual void begin();

protected:
    long t35TimeForSpeed();
    long timeForSendbytes(byte bufferSize);
    virtual void tcpPool(){};
    virtual void rtuPool();
    void sendRTUBuffer();
    void getRTURxBuffer();
    virtual void sendTCPBuffer(){};
    void onPeDePin();
    void offPeDePin();
    ModbusBridgeTCPDevice *tcpDevice;
    FLProgUart *rtuDevice;
    bool isServer = true;
    byte pinPeDe = 200;
    byte bufferSize = 0;
    byte buffer[64];
    byte lastRec = 0;
    byte workStatus = MODBUS_READY;
    unsigned long startT35;
    unsigned long startSendTime;
    int timeOfSend;

private:
};

class ModbusTcpBridge : public ModbusBridge
{
protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
    byte mbapBuffer[6];
};

class ModbusRtuOverTcpBridge : public ModbusBridge
{
protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
};

class ModbusKasCadaCloudTcpBridge : public ModbusBridge
{
public:
    virtual void pool();
    void setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    void setKaScadaCloudPort(int port);
    void setKaScadaCloudDevceId(String id);
    virtual void begin();

protected:
    virtual void tcpPool();
    virtual void sendTCPBuffer();

private:
    byte mbapBuffer[6];
    String deniceId;
    unsigned long kaScadaCloudTimeStartTime;
};