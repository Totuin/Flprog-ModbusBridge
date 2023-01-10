#include "flprogModbusBridge.h"

bool ModbusBridgeTCPDevice::setPort(int port)
{
    if (port == tcpPort)
    {
        return false;
    }

    tcpPort = port;
    return true;
}

bool ModbusBridgeTCPDevice::setRemoteIp(byte ipFirst, byte ipSecond, byte ipThird, byte ipFourth)
{

    if (compareRemoteIp(ipFirst, ipSecond, ipThird, ipFourth))
    {
        return false;
    }
    this->ipFirst = ipFirst;
    this->ipSecond = ipSecond;
    this->ipThird = ipThird;
    this->ipFourth = ipFourth;
    return true;
}

bool ModbusBridgeTCPDevice::compareRemoteIp(byte ipFirst, byte ipSecond, byte ipThird, byte ipFourth)
{
    if (this->ipFirst != ipFirst)
    {
        return false;
    }
    if (this->ipSecond != ipSecond)
    {
        return false;
    }
    if (this->ipThird != ipThird)
    {
        return false;
    }
    if (this->ipFourth != ipFourth)
    {
        return false;
    }
    return true;
}

// ModbusBridge******************
void ModbusBridge::setTCPDevice(ModbusBridgeTCPDevice *device)
{
    tcpDevice = device;
}

void ModbusBridge::setRTUDevice(FLProgUart *device)
{
    rtuDevice = device;
}

void ModbusBridge::pool()
{
    if (isServer)
    {
        tcpPool();
        rtuPool();
    }
    else
    {
        rtuPool();
        tcpPool();
    }
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
    timeOfSend = timeForSendbytes(bufferSize);
    startSendTime = millis();
    workStatus = MODBUS_WAITING_SENDING;
    bufferSize = 0;
}

long ModbusBridge::timeForSendbytes(byte bufferSize)
{
    return flprogModus::timeForSendBytes((rtuDevice->getPortDataBits()), (rtuDevice->getPortStopBits()), (rtuDevice->getPortParity()), (rtuDevice->getPortSpeed()), bufferSize);
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
    tcpDevice->begin(isServer);
}

void ModbusBridge::setTCPPort(int port)
{
    if (!(tcpDevice->setPort(port)))
    {
        return;
    }
    tcpDevice->restartServer(isServer);
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

void ModbusBridge::byServer()
{
    if (isServer == true)
    {
        return;
    }
    isServer = true;
    tcpDevice->restartServer(isServer);
}

void ModbusBridge::byClient()
{
    if (isServer == false)
    {
        return;
    }
    isServer = false;
    tcpDevice->restartServer(isServer);
}

void ModbusBridge::setTCPRemoteIp(byte ipFirst, byte ipSecond, byte ipThird, byte ipFourth)
{
    if (!tcpDevice->setRemoteIp(ipFirst, ipSecond, ipThird, ipFourth))
    {
        return;
    }
    tcpDevice->restartServer(isServer);
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
    if (!(flprog::isTimer(startT35, (t35TimeForSpeed()))))
        return;
    lastRec = 0;
    getRTURxBuffer();
    if (bufferSize < 5)
    {
        return;
    }
    sendTCPBuffer();
}

long ModbusBridge::t35TimeForSpeed()
{
    return flprogModus::t35TimeForSpeed(rtuDevice->getPortSpeed());
}

// ModbusTcpBridge******
void ModbusTcpBridge::tcpPool()
{
    tcpDevice->connect(isServer);

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

void ModbusTcpBridge::sendTCPBuffer()
{
    tcpDevice->connect(isServer);
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

// ModbusRtuOverTcpBridge*****************
void ModbusRtuOverTcpBridge::tcpPool()
{
    tcpDevice->connect(isServer);

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
            buffer[bufferSize] = tcpDevice->read();
            bufferSize++;
            byteIndex++;
        }
        sendRTUBuffer();
    }
}

void ModbusRtuOverTcpBridge::sendTCPBuffer()
{
    tcpDevice->connect(isServer);
    if (!tcpDevice->connected())
    {
        return;
    }
    tcpDevice->write(buffer, bufferSize);
    bufferSize = 0;
}

// ModbusKasCadaCloudTcpBridge*************
void ModbusKasCadaCloudTcpBridge::pool()
{
    isServer = false;
    if (flprog::isTimer(kaScadaCloudTimeStartTime, 5000))
    {
        if (tcpDevice->connected())
        {
            tcpDevice->print(deniceId);
            kaScadaCloudTimeStartTime = millis();
        }
        else
        {
            tcpDevice->connect(false);
            return;
        }
    }
    if (!tcpDevice->connected())
    {
        return;
    }
    tcpPool();
    rtuPool();
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudIp(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    isServer = false;
    if (!tcpDevice->setRemoteIp(first_octet, second_octet, third_octet, fourth_octet))
    {
        return;
    }
    tcpDevice->restartServer(false);
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudPort(int port)
{
    isServer = false;
    if (!(tcpDevice->setPort(port)))
    {
        return;
    }
    tcpDevice->restartServer(false);
}

void ModbusKasCadaCloudTcpBridge::setKaScadaCloudDevceId(String id)
{
    isServer = false;
    if (deniceId.equals(id))

    {
        return;
    }
    deniceId = id;
    tcpDevice->stop();
}

void ModbusKasCadaCloudTcpBridge::tcpPool()
{
    isServer = false;
    tcpDevice->connect(false);

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

void ModbusKasCadaCloudTcpBridge::sendTCPBuffer()
{
    isServer = false;
    tcpDevice->connect(false);
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

void ModbusKasCadaCloudTcpBridge::begin()
{
    isServer = false;
    rtuDevice->begin();
    if (!(pinPeDe == 200))
    {
        pinMode(pinPeDe, OUTPUT);
        digitalWrite(pinPeDe, LOW);
    }
    tcpDevice->begin(false);
}