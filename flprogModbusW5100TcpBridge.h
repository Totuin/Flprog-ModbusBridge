#pragma once
#include "Arduino.h"
#include "flprogModbusBridge.h"
#include "Ethernet.h"
#include "SPI.h"

class ModbusBridgeW5100TCPDevice : public ModbusBridgeTCPDevice
{
public:
    virtual void begin();
    virtual byte available();
    virtual byte read();
    virtual bool connected();
    virtual void stop();
    virtual void connect();
    virtual byte write(byte buffer[], byte size);

protected:
    virtual void restartServer();

private:
    EthernetServer *server;
    EthernetClient client;
};
