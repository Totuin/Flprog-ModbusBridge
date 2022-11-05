#include "flprogModbusHardwareRTUBridge.h"

ModbusBridgeHardwareRTUDevice::ModbusBridgeHardwareRTUDevice(HardwareSerial *port)
{
    this->port = port;
}

void ModbusBridgeHardwareRTUDevice::begin()
{
    port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
}

void ModbusBridgeHardwareRTUDevice::restartPort()
{
    port->end();
    port->begin(flprog::speedFromCode(portSpeed), flprog::serialModeFromParametrs(portDataBits, portStopBits, portParity));
}

byte ModbusBridgeHardwareRTUDevice::available()
{
    return port->available();
}

byte ModbusBridgeHardwareRTUDevice::read()
{
    return port->read();
}

byte ModbusBridgeHardwareRTUDevice::write(byte buffer[], byte size)
{
    return port->write(buffer, size);
}

bool ModbusBridgeHardwareRTUDevice::hasPort()
{
    return !(port == 0);
}