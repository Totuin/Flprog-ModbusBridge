#pragma once
#include "Arduino.h"
#include "flprogModbusBridge.h"
#include "HardwareSerial.h"

class ModbusBridgeHardwareRTUDevice : public ModbusBridgeRTUDevice
{
public:
    ModbusBridgeHardwareRTUDevice(HardwareSerial *port);
    virtual void begin();
    virtual byte available();
    virtual byte read();
    virtual byte write(byte buffer[], byte size);

protected:
    virtual bool hasPort();
    virtual void restartPort();

private:
    HardwareSerial *port;
};
