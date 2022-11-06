#pragma once
#include "Arduino.h"
#include "flprogModbusBridge.h"
#ifdef ESP32
#include "BluetoothSerial.h"
#include "flprogModbusSlaveRTU.h"

class ModbusBridgeSP32BluetootRTUhDevice : public ModbusBridgeRTUDevice
{
public:
    virtual void begin();
    virtual byte available();
    virtual byte read();
    virtual byte write(byte buffer[], byte size);
    void beBluetoothMaster();
    void beBluetoothSlave();
    void setPartnerName(String name);
    void setDeviceName(String name);

protected:
    virtual bool hasPort(){return true;};
    virtual void restartPort();

private:
    bool isMaster = false;
    String deviceName = "";
    String partnerName = "";
    BluetoothSerial port;
};

#endif