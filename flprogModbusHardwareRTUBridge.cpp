#include "flprogModbusHardwareRTUBridge.h"

ModbusBridgeHardwareRTUDevice::ModbusBridgeHardwareRTUDevice(HardwareSerial *port)
{
    this->port = port;
}

#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
ModbusBridgeHardwareRTUDevice::ModbusBridgeHardwareRTUDevice(USBSerial *port)
{
    this->usbPort = port;
    isUSB = true;
}
#endif

void ModbusBridgeHardwareRTUDevice::begin()
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)

    if (isUSB)
    {
        usbPort->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    }
    else
    {
        port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    }
#else
    port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
#endif
}

void ModbusBridgeHardwareRTUDevice::restartPort()
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)

    if (isUSB)
    {
        usbPort->end();
        usbPort->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    }
    else
    {
        port->end();
        port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
    }
#else
    port->end();
    port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
#endif
}

byte ModbusBridgeHardwareRTUDevice::available()
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    if (isUSB)
    {
        return usbPort->available();
    }
    else
    {
        return port->available();
    }

#else
    return port->available();
#endif
}

byte ModbusBridgeHardwareRTUDevice::read()
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    if (isUSB)
    {
        return usbPort->read();
    }
    else
    {
        return port->read();
    }
#else
    return port->read();
#endif
}

byte ModbusBridgeHardwareRTUDevice::write(byte buffer[], byte size)
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    if (isUSB)
    {
        return usbPort->write(buffer, size);
    }
    else
    {
        return port->write(buffer, size);
    }
#else
    return port->write(buffer, size);
#endif
}

bool ModbusBridgeHardwareRTUDevice::hasPort()
{
#if defined(_STM32_DEF_) && defined(USBCON) && defined(USBD_USE_CDC)
    if (isUSB)
    {
        return !(usbPort == 0);
    }
    else
    {
        return !(port == 0);
    }
#else
    return !(port == 0);
#endif
}