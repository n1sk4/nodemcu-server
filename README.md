# nodemcu-server
Digital GPIO control using nodeMCU ESP8266
Dinamically creates buttons for setting GPIO to HIGH or LOW over WiFi,

based on list defined in the main loop:

https://github.com/n1sk4/nodemcu-server/blob/d76a0f8e928a25c90dd90976918ed83d56a3e8e4/src/main.cpp#L7-L12

<p align="center">
<img src="https://github.com/user-attachments/assets/47545000-7c17-4bfe-9a0c-e8d154ff9865" alt="drawing" width="400"/> __  <img src="https://github.com/user-attachments/assets/ce7e18ef-509f-4391-b2da-9d880a299352" alt="drawing" width="400"/>
</p>

Done with:
 * NodeMCU(ESP3266) on espressif8266 platform
 * ESPAsyncWebServer by esphome
 * LittleFS file system
 * using PlatformIO and VS Code

## Setup
Using VS Code:
 * Install PlatformIO(PIO) extension
![image](https://github.com/user-attachments/assets/6fe7340e-5bb8-45c4-a3ee-443ef84b55f2)
 * Open git repo folder in VS Code and let PIO initialize
 * Build and Upload Filesystem Image:
   Open PIO extension in VS Code
     * -> Project Tasks -> Platform -> Build Filesystem Image
     * -> Project Tasks -> Platform -> Upload Filesystem Image
 * Change WiFi UUID and Password here:
   https://github.com/n1sk4/nodemcu-server/blob/d76a0f8e928a25c90dd90976918ed83d56a3e8e4/src/main.cpp#L15-L16
 * Define Pinout based on your needs:
   https://github.com/n1sk4/nodemcu-server/blob/d76a0f8e928a25c90dd90976918ed83d56a3e8e4/src/main.cpp#L7-L12
   
   
