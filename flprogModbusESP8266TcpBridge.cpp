#include "flprogModbusESP8266TcpBridge.h"

#ifdef ESP8266
void ModbusBridgeESP8266TCPDevice::begin(bool mode)
{
    if (mode)
    {
        server = new WiFiServer(tcpPort);
        server->begin();
    }
}

byte ModbusBridgeESP8266TCPDevice::available()
{
    if (!client.connected())
    {
        return 0;
    }
    return client.available();
}

byte ModbusBridgeESP8266TCPDevice::read()
{

    return client.read();
}

bool ModbusBridgeESP8266TCPDevice::connected()
{
    return client.connected();
}

void ModbusBridgeESP8266TCPDevice::stop()
{
    client.stop();
}

void ModbusBridgeESP8266TCPDevice::connect(bool mode)
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

byte ModbusBridgeESP8266TCPDevice::write(byte buffer[], byte size)
{
    return client.write(buffer, size);
}

void ModbusBridgeESP8266TCPDevice::restartServer(bool mode)
{
    stop();
    if (mode)
    {
        server = new WiFiServer(tcpPort);
        server->begin();
    }
    connect(mode);
}

void ModbusBridgeESP8266TCPDevice::print(String data)
{
    client.print(data);
}
#endif