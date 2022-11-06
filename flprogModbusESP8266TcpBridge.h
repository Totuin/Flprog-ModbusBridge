#pragma once
#include "Arduino.h"
#ifdef ESP8266
#include "flprogModbusBridge.h"

#include <ESP8266WiFi.h>



class ModbusBridgeESP8266TCPDevice : public ModbusBridgeTCPDevice
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