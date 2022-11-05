#include "flprogModbusBridge.h"

// ModbusBridgeRTUDevice************
void ModbusBridgeRTUDevice::setPortSpeed(byte speed)
{
    if (portSpeed == speed)
    {
        return;
    }
    portSpeed = speed;
    if (!hasPort())
    {
        return;
    }
    restartPort();
}

void ModbusBridgeRTUDevice::setPortDataBits(byte dataBits)
{
    if ((dataBits < 5) || (dataBits > 8))
    {
        return;
    }
    if (portDataBits == dataBits)
    {
        return;
    }
    portDataBits = dataBits;
    if (!hasPort())
    {
        return;
    }
    restartPort();
}

void ModbusBridgeRTUDevice::setPortStopBits(byte stopBits)
{
    if ((stopBits < 1) || (stopBits > 2))
    {
        return;
    }

    if (portStopBits == stopBits)
    {
        return;
    }
    portStopBits = stopBits;
    if (!hasPort())
    {
        return;
    }
    restartPort();
}

void ModbusBridgeRTUDevice::setPortParity(byte parity)
{
    if ((parity < 0) || (parity > 2))
    {
        return;
    }

    if (portParity == parity)
    {
        return;
    }
    portParity = parity;
    if (!hasPort())
    {
        return;
    }
    restartPort();
}

long ModbusBridgeRTUDevice::t35TimeForSpeed()
{
    return flprogModus::t35TimeForSpeed(portSpeed);
}

long ModbusBridgeRTUDevice::timeForSendbytes(byte bufferSize)
{
    return flprogModus::timeForSendBytes(portDataBits, portStopBits, portParity, portSpeed, bufferSize);
}

// ModbusBridgeTCPDevice****************
void ModbusBridgeTCPDevice::setPort(int port)
{
    if (port == tcpPort)
    {
        return;
    }

    tcpPort = port;
    restartServer();
}

// ModbusBridge******************
void ModbusBridge::setTCPDevice(ModbusBridgeTCPDevice *device)
{
    tcpDevice = device;
}

void ModbusBridge::setRTUDevice(ModbusBridgeRTUDevice *device)
{
    rtuDevice = device;
}

void ModbusBridge::pool()
{
    tcpPool();
    rtuPool();
}

void ModbusBridge::tcpPool()
{
    tcpDevice->connect();

    if (!tcpDevice->connected())
    {
        return;
    }
    byte readingByte;
    byte byteIndex = 0;
    if (tcpDevice->available())
    {
        bufferSize = 0;
        while (tcpDevice->available())
        {
            readingByte = tcpDevice->read();
            if (byteIndex < 6)
            {
                mbapBuffer[byteIndex] = readingByte;
            }
            else
            {
                buffer[bufferSize] = readingByte;
                bufferSize++;
            }
            byteIndex++;
        }
        sendRTUBuffer();
    }
}

void ModbusBridge::rtuPool()
{

    if (workStatus == MODBUS_WAITING_SENDING)
    {
        if ((flprog::isTimer(startSendTime, timeOfSend)))
        {
            workStatus = MODBUS_READY;
            offPeDePin();
        }
        else
        {
            return;
        }
    }
    byte avalibleBytes = rtuDevice->available();
    if (avalibleBytes == 0)
        return;
    if (avalibleBytes != lastRec)
    {
        lastRec = avalibleBytes;
        startT35 = millis();
        return;
    }
    if (!(flprog::isTimer(startT35, (rtuDevice->t35TimeForSpeed()))))
        return;
    lastRec = 0;
    getRTURxBuffer();
    if (bufferSize < 5)
    {
        return;
    }
    sendTCPBuffer();
}

void ModbusBridge::getRTURxBuffer()
{
    bufferSize = 0;
    while (rtuDevice->available())
    {
        buffer[bufferSize] = rtuDevice->read();
        bufferSize++;
    }
}

void ModbusBridge::sendTCPBuffer()
{
    if (!tcpDevice->connected())
    {
        return;
    }
    bufferSize -= 2;
    mbapBuffer[4] = highByte(bufferSize);
    mbapBuffer[5] = lowByte(bufferSize);
    tcpDevice->write(mbapBuffer, 6);
    tcpDevice->write(buffer, bufferSize);
    bufferSize = 0;
}

void ModbusBridge::sendRTUBuffer()
{
    onPeDePin();
    byte i = 0;
    int crc = flprogModus::modbusCalcCRC(bufferSize, buffer);
    buffer[bufferSize] = crc >> 8;
    bufferSize++;
    buffer[bufferSize] = crc & 0x00ff;
    bufferSize++;
    rtuDevice->write(buffer, bufferSize);
    timeOfSend = rtuDevice->timeForSendbytes(bufferSize);
    startSendTime = millis();
    workStatus = MODBUS_WAITING_SENDING;
    bufferSize = 0;
}

void ModbusBridge::setPinPeDe(byte pin)
{
    pinPeDe = pin;
}

void ModbusBridge::onPeDePin()
{
    if (pinPeDe == 200)
    {
        return;
    }
    digitalWrite(pinPeDe, HIGH);
}

void ModbusBridge::offPeDePin()
{
    if (pinPeDe == 200)
    {
        return;
    }
    digitalWrite(pinPeDe, LOW);
}

void ModbusBridge::begin()
{
    rtuDevice->begin();
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
    tcpDevice->begin();
}

void ModbusBridge::setTCPPort(int port)
{
    tcpDevice->setPort(port);
}

void ModbusBridge::setRtuPortSpeed(byte speed)
{
    rtuDevice->setPortSpeed(speed);
}

void ModbusBridge::setRtuPortDataBits(byte dataBits)
{
    rtuDevice->setPortDataBits(dataBits);
}

void ModbusBridge::setRtuPortStopBits(byte stopBits)
{
    rtuDevice->setPortStopBits(stopBits);
}

void ModbusBridge::setRtuPortParity(byte stopBits)
{
    rtuDevice->setPortParity(stopBits);
}
