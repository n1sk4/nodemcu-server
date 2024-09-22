#include "..\include\DigitalGPIOControl.h"

DigitalGPIOControl::DigitalGPIOControl(DigitalOutputs& outputs, AsyncWebServer& server, WiFiSettings ws)
 : m_outputs(outputs), m_server(&server), m_WiFiSettings(ws){
}

DigitalGPIOControl::DigitalGPIOControl(DigitalOutputs& outputs) 
: m_outputs(outputs){
}

bool DigitalGPIOControl::init(){
  setOutputs();
  setLow();
  
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

  m_server->on("/getLeds", HTTP_GET, [&](AsyncWebServerRequest *request) {
    String json = getLEDData();
    request->send(200, "application/json", json);
  });

  m_server->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });

  // create server listeners for on/off change based on defined m_outputs list
  repeatServerOnLED([this](long i) { offLED(i); }, m_outputs.size());
  repeatServerOnLED([this](long i) { onLED(i);  }, m_outputs.size());

  m_server->begin();

  return true;
}

void DigitalGPIOControl::initSerial(){
  Serial.begin(115200);
  Serial.print("\033[2J");  // Clear terminal
  Serial.print("\033[H");   // Move cursor to the top-left corner
}

void DigitalGPIOControl::setOutputs(){
  for(auto output : m_outputs){
    pinMode(output.pinName, OUTPUT);
  }
}

void DigitalGPIOControl::setHigh(){
  for(auto output : m_outputs){
    digitalWrite(output.pinName, HIGH);
    output.state = HIGH;
  }
}

void DigitalGPIOControl::setLow(){
  for(auto output : m_outputs){
    digitalWrite(output.pinName, LOW);
    output.state = LOW;
  }
}

void DigitalGPIOControl::setHigh(DigitalOutput& output){
  digitalWrite(output.pinName, HIGH);
  output.state = HIGH;
}

void DigitalGPIOControl::setLow(DigitalOutput& output){
  digitalWrite(output.pinName, LOW);
  output.state = LOW;
}

States DigitalGPIOControl::getStates(){
  States states;
  for(const auto& output : m_outputs){
    states.push_back(output.state);
  }
  return states;
}

uint8_t DigitalGPIOControl::getState(const DigitalOutput& output){
  return output.state;
}

String DigitalGPIOControl::processor(const String& var) {
  if (isItName(var)) 
    return ledControl(var);
  
  if (isItAdditionalData(var))
    return getLEDColor(var);

  return String();
}

bool DigitalGPIOControl::isItName(const String& var) {
  auto it = find_if(m_outputs.begin(), m_outputs.end(), [&var](const DigitalOutput& output){
    return output.name == var;
  });
  return it != m_outputs.end();
}

bool DigitalGPIOControl::isItAdditionalData(const String& var) {
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
    if(it->state == HIGH)
      return it->additionalData;
  
  return "gray"; 
}

String DigitalGPIOControl::ledControl(const String& var) {
  auto it = std::find_if(m_outputs.begin(), m_outputs.end(), [&var](const DigitalOutput& output){
    return String(output.pinName) == var;
  });

  if(it != m_outputs.end()){
    if(it->state == HIGH)
      return "ON";
    else
      return "OFF";
  }

  return String();
}

void DigitalGPIOControl::onLED(long index){
  String uriStr = "/" + String(m_outputs[index].pinName) + "on";
  const char* uri = uriStr.c_str();
  m_server->on(uri, HTTP_GET, [this, index](AsyncWebServerRequest *request) { 
    serverOnLED(m_outputs[index], HIGH, request); 
  });
}

void DigitalGPIOControl::offLED(long index){
  String uriStr = "/" + String(m_outputs[index].pinName) + "off";
  const char* uri = uriStr.c_str();
  m_server->on(uri, HTTP_GET, [this, index](AsyncWebServerRequest *request) { 
    serverOnLED(m_outputs[index], LOW, request); 
  });
}

void DigitalGPIOControl::repeatServerOnLED(onLED_t fn, long n){
  for(long i = 0; i < n; i++){
    fn(i);
  }
}

void DigitalGPIOControl::serverOnLED(DigitalOutput& pin, uint8_t state, AsyncWebServerRequest *request){
  (state == HIGH) ? setHigh(pin) : setLow(pin); 
  auto boundProcessor = std::bind(&DigitalGPIOControl::processor, this, std::placeholders::_1);
  request->send(LittleFS, "/index.html", String(), false, boundProcessor);
}

String DigitalGPIOControl::getLEDData() {
    JsonDocument jsonBuffer;  
    JsonArray leds = jsonBuffer.to<JsonArray>(); 

    for (auto &output : m_outputs) {
        JsonObject led = leds.add<JsonObject>();  
        led["id"]    = output.pinName;                  
        led["color"] = output.state == HIGH ? output.additionalData : "gray"; 
        led["name"]  = output.name;                   
        led["state"] = output.state == HIGH ? "ON" : "OFF";         
    }

    String json;
    serializeJson(jsonBuffer, json); 
    return json; 
}