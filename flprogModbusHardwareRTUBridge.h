#pragma once
#include "Arduino.h"
#include "flprogModbusBridge.h"
#include "HardwareSerial.h"

class ModbusBridgeHardwareRTUDevice : public ModbusBridgeRTUDevice
{
public:
    ModbusBridgeHardwareRTUDevice(HardwareSerial *port);
    #if defined(_STM32_DEF_) &&  defined(USBCON) && defined(USBD_USE_CDC)
    ModbusBridgeHardwareRTUDevice(USBSerial *port);
    #endif
    virtual void begin();
    virtual byte available();
    virtual byte read();
    virtual byte write(byte buffer[], byte size);

protected:
    virtual bool hasPort();
    virtual void restartPort();

private:
#if defined(_STM32_DEF_) &&  defined(USBCON) && defined(USBD_USE_CDC)
    bool isUSB = false;
    USBSerial *usbPort;
#endif
    HardwareSerial *port;
};
