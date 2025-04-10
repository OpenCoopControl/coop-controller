#include "network.h"
#include "config.h"
#include <FS.h>
#include <SPIFFS.h>
 
DeviceConfig deviceConfig;
bool configSaveNeeded = false;
 
bool initializeNetwork() {
  if (!loadDeviceConfig()) {
    setupConfigPortal();
    return true;
  }
  
  int attempts = 0;
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    if (!deviceConfig.registered) {
      if (registerDevice()) {
        deviceConfig.registered = true;
        saveDeviceConfig(deviceConfig.coopName.c_str());
      }
    }
    return true;
  } else {
    return false;
  }
}
 
void setupConfigPortal() {
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
    delay(3000);
    ESP.restart();
  }
  
  if (configSaveNeeded) {
    saveDeviceConfig(custom_coop_name.getValue());
    registerDevice();
  }
  
  digitalWrite(LED_PIN, LOW);
}
 
bool loadDeviceConfig() {
  if (!SPIFFS.exists(CONFIG_FILE)) {
    deviceConfig.deviceId = String((uint32_t)ESP.getEfuseMac(), HEX);
    deviceConfig.deviceSecret = "";
    deviceConfig.coopName = "My Chicken Coop";
    deviceConfig.registered = false;
    
    return false;
  }
  
  File file = SPIFFS.open(CONFIG_FILE, "r");
  if (!file) {
    return false;
  }
  
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();
  
  if (error) {
    return false;
  }
  
  deviceConfig.deviceId = doc["deviceId"] | String((uint32_t)ESP.getEfuseMac(), HEX);
  deviceConfig.deviceSecret = doc["deviceSecret"] | "";
  deviceConfig.coopName = doc["coopName"] | "My Chicken Coop";
  deviceConfig.registered = doc["registered"] | false;
  
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
    return false;
  }
  
  if (serializeJson(doc, file) == 0) {
    file.close();
    return false;
  }
  
  file.close();
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
  deviceConfig.deviceSecret = "secret_" + deviceConfig.deviceId + "_token";
  deviceConfig.registered = true;
  
  return saveDeviceConfig(deviceConfig.coopName.c_str());
}
 
void factoryReset() {
  if (SPIFFS.exists(CONFIG_FILE)) {
    SPIFFS.remove(CONFIG_FILE);
  }
  if (SPIFFS.exists(SETTINGS_FILE)) {
    SPIFFS.remove(SETTINGS_FILE);
  }
  
  delay(1000);
  ESP.restart();
}

bool syncWithServer() {
  return WiFi.status() == WL_CONNECTED;
}

bool sendTelemetry(float temperature, int lightLevel, int waterLevel, bool doorStatus) {
  return WiFi.status() == WL_CONNECTED;
}

bool updateFirmware() {
  return false;
}