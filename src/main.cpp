#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <map>

const char* ssid     = "12345678"; // WiFi SSID
const char* password = "12345678"; // WiFi password 
uint8_t PINS[] = {
  D5, // GPIO14
  D6, // GPIO12
  D7  // GPIO13
};

std::map <uint8_t, String> leds = {
  {PINS[0], "OFF"},
  {PINS[1], "OFF"},
  {PINS[2], "OFF"}
};
std::map <String, uint8_t> led_web_names = {
  {"LED1", PINS[0]},
  {"LED2", PINS[1]},
  {"LED3", PINS[2]}
};
std::map <String, uint8_t> led_web_colors = {
  {"RED",   PINS[0]},
  {"GREEN", PINS[1]},
  {"BLUE",  PINS[2]}
};

AsyncWebServer server(80);

String get_led_color(const String& var) {
  auto it = led_web_colors.find(var);
  if (it != led_web_colors.end()) {
    auto pin = it->second;
    if (digitalRead(pin) == LOW) // Change (LOW/HIGH) depending on the LED configuration
      return it->first;
  }
  return "gray"; 
}

String led_control(const String& var) {
  auto it = led_web_names.find(var);
  if (it != led_web_names.end()) {
    auto pin = it->second;
    if (digitalRead(pin) == LOW) // Change (LOW/HIGH) depending on the LED configuration
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

void server_on_led(uint8_t led_pin, uint8_t state, AsyncWebServerRequest *request){
  digitalWrite(led_pin, state);
  request->send(LittleFS, "/index.html", String(), false, processor);
}

void setup() {
  Serial.begin(115200);
  for (auto led : leds) {
    pinMode(led.first, OUTPUT);
    digitalWrite(led.first, HIGH); // Change (LOW/HIGH) depending on the LED configuration
  }

  Serial.print("\033[2J");  // Clear terminal
  Serial.print("\033[H");   // Move cursor to the top-left corner

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
    server_on_led(PINS[0], LOW, request); // Change (LOW/HIGH) depending on the LED configuration
  });
  server.on("/led1off", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(PINS[0], HIGH, request); // Change (LOW/HIGH) depending on the LED configuration
  });

  server.on("/led2on", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(PINS[1], LOW, request); // Change (LOW/HIGH) depending on the LED configuration
  });
  server.on("/led2off", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(PINS[1], HIGH, request); // Change (LOW/HIGH) depending on the LED configuration
  });

  server.on("/led3on", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(PINS[2], LOW, request); // Change (LOW/HIGH) depending on the LED configuration
  });
  server.on("/led3off", HTTP_GET, [](AsyncWebServerRequest *request) {
    server_on_led(PINS[2], HIGH, request); // Change (LOW/HIGH) depending on the LED configuration
  });

  server.begin();
}

void loop() {
  // do nothing
}
