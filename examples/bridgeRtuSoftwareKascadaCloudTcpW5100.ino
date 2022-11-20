#include "flprogModbusSoftwareRTUBridge.h"
#include "flprogModbusW5100TcpBridge.h"
#include "flprogModbusBridge.h"

byte ethernet_mac [] = {0x78, 0xAC, 0xC0, 0x7E, 0xC1, 0x01};
IPAddress ethernet_ip(192, 168, 0, 50);
byte ethernet_dns [] = {192, 168, 0, 1};
byte ethernet_gateway [] = {192, 168, 0, 1};
byte ethernet_subnet [] = {255, 255, 255, 0};

ModbusKasCadaCloudTcpBridge _Bridge;

ModbusBridgeSoftwareRTUDevice RtuDevice(2, 3);

ModbusBridgeW5100TCPDevice TcpDevice;

void setup() {

  Ethernet.begin(ethernet_mac, ethernet_ip, ethernet_dns, ethernet_gateway, ethernet_subnet);
  delay(1000);

  _Bridge.setRTUDevice(&RtuDevice);
  _Bridge.setTCPDevice(&TcpDevice);
  _Bridge.setKaScadaCloudIp(94, 250, 249, 225);
  _Bridge.setKaScadaCloudPort(25000);
  _Bridge.setKaScadaCloudDevceId("DKK6T9TPE4");
  _Bridge.begin();
}

void loop() {
  _Bridge.pool();


}