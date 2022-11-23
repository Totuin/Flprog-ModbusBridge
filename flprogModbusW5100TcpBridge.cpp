#include "flprogModbusW5100TcpBridge.h"

#ifndef ESP32
void ModbusBridgeW5100TCPDevice::begin(bool mode)
{
    if (mode)
    {
        server = new EthernetServer(tcpPort);
        server->begin();
    }
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

void ModbusBridgeW5100TCPDevice::connect(bool mode)
{
    if (!client.connected())
    {
        if (mode)
        {
            client = server->available();
        }
        else
        {
            client.connect(IPAddress(ipFirst, ipSecond, ipThird, ipFourth), tcpPort);
        }
    }
}

byte ModbusBridgeW5100TCPDevice::write(byte buffer[], byte size)
{
    return client.write(buffer, size);
}

void ModbusBridgeW5100TCPDevice::restartServer(bool mode)
{
    stop();
    if (mode)
    {
        server = new EthernetServer(tcpPort);
        server->begin();
    }
}

void ModbusBridgeW5100TCPDevice::print(String data)
{
    client.print(data);
}

#endif