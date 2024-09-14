# nodemcu-server
LED light control using a NodeMCU as the server


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
 * Change these two lines to match your WiFi and it's password:
https://github.com/n1sk4/nodemcu-server/blob/662db253dc016a46c92e234adbb7616733e5beaa/src/main.cpp#L9-L10
 * Change D0, D1, D2 with your GPIO's here:
https://github.com/n1sk4/nodemcu-server/blob/662db253dc016a46c92e234adbb7616733e5beaa/src/main.cpp#L12-L26
 * Build and Upload the project

