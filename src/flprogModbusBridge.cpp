#include "flprogModbusBridge.h"

void ModbusBridge::pool()
{
    TcpPool();
    RtuPool();
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
    byte i = 0;
    int crc = flprogModus::modbusCalcCRC(bufferSize, buffer);
    buffer[bufferSize] = crc >> 8;
    bufferSize++;
    buffer[bufferSize] = crc & 0x00ff;
    bufferSize++;
    rtuDevice->write(buffer, bufferSize);
    bufferSize = 0;
}