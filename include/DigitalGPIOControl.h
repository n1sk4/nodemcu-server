#ifndef DIGITAL_GPIO_CONTROL_H
#define DIGITAL_GPIO_CONTROL_H

#pragma once

#include <Arduino.h>
#include <map>
#include <vector>

struct DigitalOutput{
  uint8_t pinName;
  String  name;
  String  additionalData;
  uint8_t    state;
};

typedef std::vector<DigitalOutput> DigitalOutputs;
typedef std::vector<uint8_t> States;

class DigitalGPIOControl{
public:
  DigitalGPIOControl(DigitalOutputs& outputs);

  void SetHigh();
  void SetLow();
  void SetHigh(DigitalOutput& output);
  void SetLow(DigitalOutput& output);
  States GetStates();
  uint8_t GetState(const DigitalOutput& output);
  bool IsItName(const String& var);
  bool IsItAdditionalData(const String& var); 
  void Init();
private:
  DigitalOutputs m_outputs;
  void SetOutputs();
};

#endif