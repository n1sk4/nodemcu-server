#include "..\include\DigitalGPIOControl.h"

// Pinout
DigitalOutputs pins = {
// pin,   name ,  data  , state
   {D5,  "LED1", "red"  , LOW}, // GPIO14
   {D6,  "LED2", "green", LOW}, // GPIO12
   {D7,  "LED3", "blue" , LOW}, // GPIO13
};

// Server - WiFi
#define WIFI_SSID       "12345678" // WiFi SSID
#define WIFI_PASSWORD   "12345678" // WiFi PASSWORD

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST_PIN);

WiFiSettings ws{
  WIFI_SSID,
  WIFI_PASSWORD
};

AsyncWebServer server(80);
DigitalGPIOControl ctrl(pins, server, ws, display);

void setup() {
  if (!ctrl.init()){
    Serial.println("Error initializing DigitalGPIOControl");
    return;
  }

  delay(2000);
}

int frame = 0;
void loop() {
  // do something
  display.clearDisplay();
  display.drawBitmap(32, 0, RunningWebAnimation[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
  display.display();
  frame = (frame + 1) % FRAME_COUNT;
  delay(42);
}
