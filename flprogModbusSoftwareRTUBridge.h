#pragma once
#include "Arduino.h"
#ifndef ESP32
#include "flprogModbusBridge.h"
#include "SoftwareSerial.h"

class ModbusBridgeSoftwareRTUDevice : public ModbusBridgeRTUDevice
{
public:
    ModbusBridgeSoftwareRTUDevice(uint8_t receivePin, uint8_t transmitPin);
    virtual void begin();
    virtual byte available();
    virtual byte read();
    virtual byte write(byte buffer[], byte size);

protected:
    virtual bool hasPort();
    virtual void restartPort();

private:
    SoftwareSerial *port;
};
#endif