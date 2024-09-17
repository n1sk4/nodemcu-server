#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <algorithm>

#include "..\include\DigitalGPIOControl.h"

const char* ssid     = "12345678"; // WiFi SSID
const char* password = "12345678"; // WiFi password 

DigitalOutputs pins = {
// pin,  name,   data  , state
  {D5,  "LED1", "RED"  , LOW}, // GPIO14
  {D6,  "LED2", "GREEN", LOW}, // GPIO12
  {D7,  "LED3", "BLUE" , LOW}  // GPIO13
};

DigitalGPIOControl p(pins);
AsyncWebServer server(80);

String get_led_color(const String& var) {
  auto it = find_if(pins.begin(), pins.end(), [&var](const DigitalOutput& output){
    Serial.println(var);
    return output.additionalData == var;
  });

  if(it != pins.end())
    if(it->state)
      return it->additionalData;
  
  return "gray"; 
}

String led_control(const String& var) {
  auto it = find_if(pins.begin(), pins.end(), [&var](const DigitalOutput& output){
    return output.name == var;
  });

  if(it != pins.end()){
    if(it->state)
      return "ON";
    else
      return "OFF";
  }

  return String();
}

String processor(const String& var) {
  if (p.IsItName(var)) 
    return led_control(var);
  
  if (p.IsItAdditionalData(var))
    return get_led_color(var);

  return String();
}

void server_on_led(DigitalOutput& pin, uint8_t state, AsyncWebServerRequest *request){
  (state == HIGH) ? p.SetHigh(pin) : p.SetLow(pin); 
  request->send(LittleFS, "/index.html", String(), false, processor);
}


void initSerial(){
  Serial.begin(115200);
  Serial.print("\033[2J");  // Clear terminal
  Serial.print("\033[H");   // Move cursor to the top-left corner
}

void setup() {
  p.Init();
  initSerial();

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\n");
  Serial.print("Client available at: ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });
  
  server.on("/led1on", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(pins[0], LOW, request); // Change (LOW/HIGH) depending on the LED configuration
  });
  server.on("/led1off", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(pins[0], HIGH, request); // Change (LOW/HIGH) depending on the LED configuration
  });

  server.on("/led2on", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(pins[1], LOW, request); // Change (LOW/HIGH) depending on the LED configuration
  });
  server.on("/led2off", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(pins[1], HIGH, request); // Change (LOW/HIGH) depending on the LED configuration
  });

  server.on("/led3on", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(pins[2], LOW, request); // Change (LOW/HIGH) depending on the LED configuration
  });
  server.on("/led3off", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(pins[2], HIGH, request); // Change (LOW/HIGH) depending on the LED configuration
  });

  server.begin();
}

void loop() {
  // do nothing
}
