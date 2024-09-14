#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <vector>
#include <map>

const char* ssid     = "12345678"; // WiFi SSID
const char* password = "12345678"; // WiFi password 

std::map <uint8_t, String> leds = {
  {D0, "OFF"},
  {D1, "OFF"},
  {D2, "OFF"}
};
std::map <String, uint8_t> led_web_names = {
  {"LED1", D0},
  {"LED2", D1},
  {"LED3", D2}
};
std::map <String, uint8_t> led_web_colors = {
  {"RED",   D0},
  {"GREEN", D1},
  {"BLUE",  D2}
};

AsyncWebServer server(80);

String get_led_color(const String& var) {
  auto it = led_web_colors.find(var);
  if (it != led_web_colors.end()) {
    auto pin = it->second;
    if (digitalRead(pin) == LOW) 
      return it->first;
  }
  return "gray"; 
}

String led_control(const String& var) {
  auto it = led_web_names.find(var);
  if (it != led_web_names.end()) {
    auto pin = it->second;
    if (digitalRead(pin) == LOW)
      leds[pin] = "ON";
    else  
      leds[pin] = "OFF";

    return leds[pin];
  }
  return String();
}

bool is_led(const String& var) {
  return led_web_names.find(var) != led_web_names.end();
}
bool is_led_color(const String& var) {
  return led_web_colors.find(var) != led_web_colors.end();
}

String processor(const String& var) {
  if (is_led(var)) 
    return led_control(var);
  
  if (is_led_color(var))
    return get_led_color(var);

  return String();
}

void setup() {
  Serial.begin(115200);
  for (auto led : leds) {
    pinMode(led.first, OUTPUT);
    digitalWrite(led.first, HIGH); 
  }

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

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/led1on", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(D0, LOW);
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/led1off", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(D0, HIGH);
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/led2on", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(D1, LOW);
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/led2off", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(D1, HIGH); 
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/led3on", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(D2, LOW);
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/led3off", HTTP_GET, [](AsyncWebServerRequest *request) {
    digitalWrite(D2, HIGH);
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.begin();
}

void loop() {
}
