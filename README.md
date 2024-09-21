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
 https://github.com/n1sk4/nodemcu-server/blob/d1f40ccb82a65ed970c7abc552b5c00be45f87e3/src/main.cpp#L8-L9

 * Change D5, D6, D7 with your GPIO's here:
 https://github.com/n1sk4/nodemcu-server/blob/18c87bc8783e3671fa59fd175468bad393550cb9/src/main.cpp#L16-L18
 * Build and Upload the project

LED's in the project are connected as seen in the schematics bellow (I had a RGB LED that was configured with a common anode)
![nodemcu-server](https://github.com/user-attachments/assets/d19526ea-40ce-4efc-8679-5ae46fcfa7a6)
