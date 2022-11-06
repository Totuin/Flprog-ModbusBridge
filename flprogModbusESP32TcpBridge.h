#pragma once
#include "Arduino.h"
#include "flprogModbusBridge.h"
#ifdef ESP32
#include "WiFi.h"

class ModbusBridgeESP32TCPDevice : public ModbusBridgeTCPDevice
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
    WiFiServer *server;
    WiFiClient client;
};

#endif