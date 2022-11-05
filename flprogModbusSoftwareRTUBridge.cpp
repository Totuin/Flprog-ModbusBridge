#include "flprogModbusSoftwareRTUBridge.h"

ModbusBridgeSoftwareRTUDevice::ModbusBridgeSoftwareRTUDevice(uint8_t receivePin, uint8_t transmitPin)
{
    port = new SoftwareSerial(receivePin, transmitPin);
    
}

void ModbusBridgeSoftwareRTUDevice::begin()
{
    port->begin(flprog::speedFromCode(portSpeed));
}

byte ModbusBridgeSoftwareRTUDevice::available()
{
    return port->available();
}

byte ModbusBridgeSoftwareRTUDevice::read()
{
    return port->read();
}

byte ModbusBridgeSoftwareRTUDevice::write(byte buffer[], byte size)
{
    return port->write(buffer, size);
}

bool ModbusBridgeSoftwareRTUDevice::hasPort()
{
    return !(port == 0);
}

void ModbusBridgeSoftwareRTUDevice::restartPort()
{
    port->end();
    port->begin(flprog::speedFromCode(portSpeed));
}
