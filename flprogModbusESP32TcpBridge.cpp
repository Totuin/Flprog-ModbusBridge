#include "flprogModbusESP32TcpBridge.h"
#ifdef ESP32
void ModbusBridgeESP32TCPDevice::begin(bool mode)
{
    restartServer(mode);
}

byte ModbusBridgeESP32TCPDevice::available()
{
    if (!client.connected())
    {
        return 0;
    }
    return client.available();
}

byte ModbusBridgeESP32TCPDevice::read()
{

    return client.read();
}

bool ModbusBridgeESP32TCPDevice::connected()
{
    return client.connected();
}

void ModbusBridgeESP32TCPDevice::stop()
{
    client.stop();
}

void ModbusBridgeESP32TCPDevice::connect(bool mode)
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

byte ModbusBridgeESP32TCPDevice::write(byte buffer[], byte size)
{
    return client.write(buffer, size);
}

void ModbusBridgeESP32TCPDevice::restartServer(bool mode)
{
    stop();
    if (server)
    {
        server->stop();
    }
    if (mode)
    {
        server = new WiFiServer(tcpPort);
        server->begin();
    }
}

void ModbusBridgeESP32TCPDevice::print(String data)
{
    client.print(data);
}

#endif