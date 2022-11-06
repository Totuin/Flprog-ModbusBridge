#include "flprogModbusRTUESP32BluetoothBridge.h"

#ifdef ESP32

void ModbusBridgeSP32BluetootRTUhDevice::begin()
{
    port.begin(deviceName);
    if (isMaster)
    {
        port.connect(partnerName);
    }
}

byte ModbusBridgeSP32BluetootRTUhDevice::available()
{
    return port.available();
}

byte ModbusBridgeSP32BluetootRTUhDevice::read()
{
    return port.read();
}

byte ModbusBridgeSP32BluetootRTUhDevice::write(byte buffer[], byte size)
{
    return port.write(buffer, size);
}

void ModbusBridgeSP32BluetootRTUhDevice::restartPort()
{
    port.end();
    port.begin(deviceName);
    if (isMaster)
    {
        port.connect(partnerName);
    }
}

void ModbusBridgeSP32BluetootRTUhDevice::beBluetoothMaster()
{
    if (isMaster)
    {
        return;
    }
    isMaster = true;
    restartPort();
}

void ModbusBridgeSP32BluetootRTUhDevice::beBluetoothSlave()
{
    if (!isMaster)
    {
        return;
    }
    isMaster = false;
    restartPort();
}

void ModbusBridgeSP32BluetootRTUhDevice::setPartnerName(String name)
{
    if (partnerName.equals(name))
    {
        return;
    }
    partnerName = name;
    restartPort();
}

void ModbusBridgeSP32BluetootRTUhDevice::setDeviceName(String name)
{
    if (deviceName.equals(name))
    {
        return;
    }
    deviceName = name;
    restartPort();
}

#endif