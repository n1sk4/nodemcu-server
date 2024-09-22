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
typedef std::function<void(long)> onLED_t;

class DigitalGPIOControl{
public:
  DigitalGPIOControl(DigitalOutputs& outputs);
  DigitalGPIOControl(DigitalOutputs& outputs, AsyncWebServer& server, WiFiSettings ws);

  bool init();
  void setHigh();
  void setLow();
  void setHigh(DigitalOutput& output);
  void setLow(DigitalOutput& output);
  States getStates();
  uint8_t getState(const DigitalOutput& output);

private:
  DigitalOutputs  m_outputs;
  AsyncWebServer* m_server;
  WiFiSettings    m_WiFiSettings;
  
  void setOutputs();
  String getLEDData();
  void repeatServerOnLED(onLED_t fn, long n);
  void onLED(long index);
  void offLED(long index);
  String ledControl(const String& var);
  String getLEDColor(const String& var);
  String processor(const String& var);
  void serverOnLED(DigitalOutput& pin, uint8_t state, AsyncWebServerRequest *request);
  bool isItName(const String& var);
  bool isItAdditionalData(const String& var); 
  void initSerial();
};

#endif