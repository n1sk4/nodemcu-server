#ifndef DIGITAL_GPIO_CONTROL_H
#define DIGITAL_GPIO_CONTROL_H

#pragma once

#include <Arduino.h>
#include <map>
#include <vector>
#include <algorithm>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <functional>

struct DigitalOutput{
  uint8_t pinName;
  String  name;
  String  additionalData;
  uint8_t state;
};

struct WiFiSettings{
  const char* ssid;
  const char* password;
};

typedef std::vector<DigitalOutput> DigitalOutputs;
typedef std::vector<uint8_t> States;

class DigitalGPIOControl{
public:
  DigitalGPIOControl(DigitalOutputs& outputs);
  DigitalGPIOControl(DigitalOutputs& outputs, AsyncWebServer& server, WiFiSettings ws);

  bool init();
  
  void SetHigh();
  void SetLow();
  void SetHigh(DigitalOutput& output);
  void SetLow(DigitalOutput& output);
  States GetStates();
  uint8_t GetState(const DigitalOutput& output);
  bool IsItName(const String& var);
  bool IsItAdditionalData(const String& var); 

  //Server
  void setSSID(const char& ssid);
  void setPassword(const char& password);
  String LEDControl(const String& var);
  String getLEDColor(const String& var);
  String processor(const String& var);
  void serverOnLED(DigitalOutput& pin, uint8_t state, AsyncWebServerRequest *request);
  void initSerial();
private:
  DigitalOutputs  m_outputs;
  AsyncWebServer* m_server;
  WiFiSettings    m_WiFiSettings;
  void SetOutputs();
  String getLEDData();

  const char* uri;
  String temp;
  void onLED(long index);
  void offLED(long index);
  typedef std::function<void(long)> onLED_t;
  void repeatServerOnLED(onLED_t fn, long n);
};

#endif