#pragma once
#include "Arduino.h"
#include "flprogModbusUtilites.h"

class ModbusBridge
{
public:
    void pool();

protected:
private:
    void tcpPool();
    void rtuPool();
    void sendRTUBuffer();
    void getRTURxBuffer();
    void sendTCPBuffer();
    byte bufferSize = 0;
    byte mbapBuffer[6];
    byte buffer[64];
    byte lastRec = 0;
    unsigned long startT35;
    ModbusBridgeTCPDevice *tcpDevice;
    ModbusBridgeRTUDevice *rtuDevice;
};

class ModbusBridgeTCPDevice
{
public:
    virtual byte available(){};
    virtual byte read(){};
    virtual bool connected(){};
    virtual void stop(){};
    virtual void connect(){};
    virtual byte write(byte buffer[], byte size){};

protected:
private:
};

class ModbusBridgeRTUDevice
{
public:
    virtual byte available(){};
    virtual byte read(){};
    virtual unsigned long t35TimeForSpeed(){};
    virtual byte write(byte buffer[], byte size){};

protected:
private:
};