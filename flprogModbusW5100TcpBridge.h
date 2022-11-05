#pragma once
#include "Arduino.h"
#include "flprogModbusBridge.h"
#include "Ethernet.h"
#include "SPI.h"

class ModbusBridgeW5100TCPDevice : public ModbusBridgeTCPDevice
{
public:
    virtual void begin(bool mode);
    virtual byte available();
    virtual byte read();
    virtual bool connected();
    virtual void stop();
    virtual void connect(bool mode);
    virtual byte write(byte buffer[], byte size);
    virtual void restartServer(bool mode);
    virtual void print(String data);

protected:
private:
    EthernetServer *server;
    EthernetClient client;
};
