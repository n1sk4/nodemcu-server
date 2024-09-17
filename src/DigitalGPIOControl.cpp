#include "..\include\DigitalGPIOControl.h"

DigitalGPIOControl::DigitalGPIOControl(DigitalOutputs& outputs) : m_outputs(outputs){
}

void DigitalGPIOControl::Init(){
  SetOutputs();
  SetLow();
}

void DigitalGPIOControl::SetOutputs(){
  for(auto output : m_outputs){
    pinMode(output.pinName, OUTPUT);
  }
}

void DigitalGPIOControl::SetHigh(){
  for(auto output : m_outputs){
    digitalWrite(output.pinName, HIGH);
    output.state = HIGH;
  }
}

void DigitalGPIOControl::SetLow(){
  for(auto output : m_outputs){
    digitalWrite(output.pinName, LOW);
    output.state = LOW;
  }
}

void DigitalGPIOControl::SetHigh(DigitalOutput& output){
  digitalWrite(output.pinName, HIGH);
  output.state = HIGH;
}

void DigitalGPIOControl::SetLow(DigitalOutput& output){
  digitalWrite(output.pinName, LOW);
  output.state = LOW;
}

States DigitalGPIOControl::GetStates(){
  States states;
  for(const auto& output : m_outputs){
    states.push_back(output.state);
  }
  return states;
}

uint8_t DigitalGPIOControl::GetState(const DigitalOutput& output){
  return output.state;
}

bool DigitalGPIOControl::IsItName(const String& var) {
  auto it = find_if(m_outputs.begin(), m_outputs.end(), [&var](const DigitalOutput& output){
    Serial.println(output.name);
    return output.name == var;
  });
  Serial.println("here");
  return it != m_outputs.end();
}

bool DigitalGPIOControl::IsItAdditionalData(const String& var) {
  auto it = find_if(m_outputs.begin(), m_outputs.end(), [&var](const DigitalOutput& output){
    Serial.println(output.additionalData);
    return output.additionalData == var;
  });
  Serial.println("there");
  return it != m_outputs.end();
}
