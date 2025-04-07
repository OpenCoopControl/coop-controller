 #ifndef NETWORK_H
 #define NETWORK_H
 
 #include <Arduino.h>
 #include <WiFi.h>
 #include <WiFiManager.h>
 #include <ArduinoJson.h>
 #include <SPIFFS.h>
 #include <WebServer.h>
 
 struct DeviceConfig {
     String deviceId;
     String deviceSecret;
     String coopName;
     bool registered;
 };
 
 bool initializeNetwork();
 void setupConfigPortal();
 bool loadDeviceConfig();
 bool saveDeviceConfig(const char* coopName);
 String getDeviceId();
 String getCoopName();
 bool isDeviceConfigured();
 bool isDeviceRegistered();
 bool registerDevice();
 void factoryReset();
 
 bool syncWithServer();
 bool sendTelemetry(float temperature, int lightLevel, int waterLevel, bool doorStatus);
 bool updateFirmware();
 
 #endif