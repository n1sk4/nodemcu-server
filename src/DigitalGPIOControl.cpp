#include "..\include\DigitalGPIOControl.h"

DigitalGPIOControl::DigitalGPIOControl(DigitalOutputs& outputs, AsyncWebServer& server, WiFiSettings ws)
 : m_outputs(outputs), m_server(&server), m_WiFiSettings(ws){
}

DigitalGPIOControl::DigitalGPIOControl(DigitalOutputs& outputs) 
: m_outputs(outputs){
}

bool DigitalGPIOControl::init(){
  SetOutputs();
  SetLow();
  
  initSerial();

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return false;
  }
  
  if(!m_server || !m_WiFiSettings.ssid || !m_WiFiSettings.password){
    Serial.println("An Error has occurred while setting up server");
    return true;
  }

  WiFi.begin(m_WiFiSettings.ssid, m_WiFiSettings.password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\n");
  Serial.print("Client available at: ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");

  m_server->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
    auto boundProcessor = std::bind(&DigitalGPIOControl::processor, this, std::placeholders::_1);
    request->send(LittleFS, "/index.html", String(), false, boundProcessor);
  });

  m_server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });
  
  SERVER_ON_LED(1)
  SERVER_ON_LED(2)
  SERVER_ON_LED(3)

  m_server->begin();

  return true;
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
    return output.name == var;
  });
  return it != m_outputs.end();
}

bool DigitalGPIOControl::IsItAdditionalData(const String& var) {
  auto it = find_if(m_outputs.begin(), m_outputs.end(), [&var](const DigitalOutput& output){
    return output.additionalData == var;
  });
  return it != m_outputs.end();
}

String DigitalGPIOControl::getLEDColor(const String& var) {
  auto it = std::find_if(m_outputs.begin(), m_outputs.end(), [&var](const DigitalOutput& output){
    return output.additionalData == var;
  });

  if(it != m_outputs.end())
    if(it->state)
      return it->additionalData;
  
  return "gray"; 
}

String DigitalGPIOControl::LEDControl(const String& var) {
  auto it = std::find_if(m_outputs.begin(), m_outputs.end(), [&var](const DigitalOutput& output){
    return output.name == var;
  });

  if(it != m_outputs.end()){
    if(it->state)
      return "ON";
    else
      return "OFF";
  }

  return String();
}

String DigitalGPIOControl::processor(const String& var) {
  if (IsItName(var)) 
    return LEDControl(var);
  
  if (IsItAdditionalData(var))
    return getLEDColor(var);

  return String();
}

void DigitalGPIOControl::serverOnLED(DigitalOutput& pin, uint8_t state, AsyncWebServerRequest *request){
  (state == HIGH) ? SetHigh(pin) : SetLow(pin); 
  auto boundProcessor = std::bind(&DigitalGPIOControl::processor, this, std::placeholders::_1);
  request->send(LittleFS, "/index.html", String(), false, boundProcessor);
}


void DigitalGPIOControl::initSerial(){
  Serial.begin(115200);
  Serial.print("\033[2J");  // Clear terminal
  Serial.print("\033[H");   // Move cursor to the top-left corner
}
