 #include "network.h"
 #include "config.h"
 #include <FS.h>
 #include <SPIFFS.h>
 
 DeviceConfig deviceConfig;
 bool configSaveNeeded = false;
 
 bool initializeNetwork() {
   Serial.println("Initializing network...");
   
   if (!loadDeviceConfig()) {
     setupConfigPortal();
     return true;
   }
   
   Serial.print("Connecting to WiFi");
   
   int attempts = 0;
   WiFi.begin();
   while (WiFi.status() != WL_CONNECTED && attempts < 20) {
     delay(500);
     Serial.print(".");
     attempts++;
   }
   
   if (WiFi.status() == WL_CONNECTED) {
     Serial.println("\nWiFi connected");
     Serial.print("IP address: ");
     Serial.println(WiFi.localIP());
     
     if (!deviceConfig.registered) {
       if (registerDevice()) {
         deviceConfig.registered = true;
         saveDeviceConfig(deviceConfig.coopName.c_str());
       }
     }
     return true;
   } else {
     Serial.println("\nFailed to connect to WiFi");
     return false;
   }
 }
 
 void setupConfigPortal() {
   Serial.println("Starting configuration portal");
   digitalWrite(LED_PIN, HIGH);
   
   WiFiManager wifiManager;
   
   WiFiManagerParameter custom_coop_name("coop_name", "Coop Name (optional)", deviceConfig.coopName.c_str(), 40);
   wifiManager.addParameter(&custom_coop_name);
   
   wifiManager.setConfigPortalTimeout(AP_TIMEOUT);
   
   wifiManager.setSaveConfigCallback([]() {
     configSaveNeeded = true;
   });
   
   String apName = AP_NAME_PREFIX + String((uint32_t)ESP.getEfuseMac(), HEX);
   
   if (!wifiManager.startConfigPortal(apName.c_str())) {
     Serial.println("Failed to connect or timeout");
     delay(3000);
     ESP.restart();
   }
   
   Serial.println("Connected to WiFi");
   
   if (configSaveNeeded) {
     saveDeviceConfig(custom_coop_name.getValue());
     registerDevice();
   }
   
   digitalWrite(LED_PIN, LOW);
 }
 
 bool loadDeviceConfig() {
   if (!SPIFFS.exists(CONFIG_FILE)) {
     Serial.println("No config file found");
     
     deviceConfig.deviceId = String((uint32_t)ESP.getEfuseMac(), HEX);
     deviceConfig.deviceSecret = "";
     deviceConfig.coopName = "My Chicken Coop";
     deviceConfig.registered = false;
     
     return false;
   }
   
   File file = SPIFFS.open(CONFIG_FILE, "r");
   if (!file) {
     Serial.println("Failed to open config file");
     return false;
   }
   
   JsonDocument doc;
   DeserializationError error = deserializeJson(doc, file);
   file.close();
   
   if (error) {
     Serial.println("Failed to parse config file");
     return false;
   }
   
   deviceConfig.deviceId = doc["deviceId"] | String((uint32_t)ESP.getEfuseMac(), HEX);
   deviceConfig.deviceSecret = doc["deviceSecret"] | "";
   deviceConfig.coopName = doc["coopName"] | "My Chicken Coop";
   deviceConfig.registered = doc["registered"] | false;
   
   Serial.println("Device config loaded");
   Serial.print("Device ID: ");
   Serial.println(deviceConfig.deviceId);
   Serial.print("Coop Name: ");
   Serial.println(deviceConfig.coopName);
   
   return true;
 }
 
 bool saveDeviceConfig(const char* coopName) {
   JsonDocument doc;
   
   if (coopName && strlen(coopName) > 0) {
     deviceConfig.coopName = coopName;
   }
   
   doc["deviceId"] = deviceConfig.deviceId;
   doc["deviceSecret"] = deviceConfig.deviceSecret;
   doc["coopName"] = deviceConfig.coopName;
   doc["registered"] = deviceConfig.registered;
   
   File file = SPIFFS.open(CONFIG_FILE, "w");
   if (!file) {
     Serial.println("Failed to open config file for writing");
     return false;
   }
   
   if (serializeJson(doc, file) == 0) {
     Serial.println("Failed to write config to file");
     file.close();
     return false;
   }
   
   file.close();
   Serial.println("Config saved successfully");
   return true;
 }
 
 String getDeviceId() {
   return deviceConfig.deviceId;
 }
 
 String getCoopName() {
   return deviceConfig.coopName;
 }
 
 bool isDeviceConfigured() {
   return SPIFFS.exists(CONFIG_FILE);
 }
 
 bool isDeviceRegistered() {
   return deviceConfig.registered;
 }
 
 bool registerDevice() {   
   Serial.println("Registering device with backend...");
   Serial.println("Device ID: " + deviceConfig.deviceId);
   /*Simulate until back end is implemented*/
   deviceConfig.deviceSecret = "secret_" + deviceConfig.deviceId + "_token";
   deviceConfig.registered = true;
   
   Serial.println("Device registered successfully!");
   Serial.println("IMPORTANT: Note your device ID for adding to the app:");
   Serial.println(deviceConfig.deviceId);
   
   return saveDeviceConfig(deviceConfig.coopName.c_str());
 }
 
 void factoryReset() {
   Serial.println("Performing factory reset...");
   
   if (SPIFFS.exists(CONFIG_FILE)) {
     SPIFFS.remove(CONFIG_FILE);
   }
   if (SPIFFS.exists(SETTINGS_FILE)) {
     SPIFFS.remove(SETTINGS_FILE);
   }
   
   Serial.println("Factory reset complete. Restarting...");
   delay(1000);
   ESP.restart();
 }
 
 bool syncWithServer() {
   Serial.println("Syncing with server...");
   return WiFi.status() == WL_CONNECTED;
 }
 
 bool sendTelemetry(float temperature, int lightLevel, int waterLevel, bool doorStatus) {
   Serial.println("Sending telemetry data...");
   
   Serial.print("Temperature: "); Serial.print(temperature); Serial.println("°C");
   Serial.print("Light level: "); Serial.print(lightLevel); Serial.println("%");
   Serial.print("Water level: "); Serial.print(waterLevel); Serial.println("%");
   Serial.print("Door status: "); Serial.println(doorStatus ? "Open" : "Closed");
   
   return WiFi.status() == WL_CONNECTED;
 }
 
 bool updateFirmware() {
   Serial.println("Checking for firmware updates...");
   return false; 
 }