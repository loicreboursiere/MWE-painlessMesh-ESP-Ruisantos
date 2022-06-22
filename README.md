# Test of painlessMesh for ESP32 & ESP8266

Based on https://randomnerdtutorials.com/esp-mesh-esp32-esp8266-painlessmesh/

## ESPAsyncTCP

It seems like `ESPAsyncTCP` is not reachable through Arduino librairies manager.
Need to download it manually.
```
cd ~/Documents/Arduino/librairies
git clone https://github.com/me-no-dev/ESPAsyncTCP.git
``` 

Restart (not sure) and re-upload script.