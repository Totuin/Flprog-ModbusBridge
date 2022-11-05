#include "flprogModbusW5100TcpBridge.h"

void ModbusBridgeW5100TCPDevice::begin()
{
    server = new EthernetServer(tcpPort);
    server->begin();
}

byte ModbusBridgeW5100TCPDevice::available()
{
    if (!client.connected())
    {
        return 0;
    }
    return client.available();
}

byte ModbusBridgeW5100TCPDevice::read()
{

    return client.read();
}

bool ModbusBridgeW5100TCPDevice::connected()
{
    return client.connected();
}

void ModbusBridgeW5100TCPDevice::stop()
{
    client.stop();
}

void ModbusBridgeW5100TCPDevice::connect()
{
    if (!client.connected())
    {
        client = server->available();
    }
}

byte ModbusBridgeW5100TCPDevice::write(byte buffer[], byte size)
{
    return client.write(buffer, size);
}

void ModbusBridgeW5100TCPDevice::restartServer()
{
    stop();
    server = new EthernetServer(tcpPort);
    server->begin();
    connect();
}