 #include <Arduino.h>
 #include <TimeLib.h>
 #include <FS.h>
 #include <SPIFFS.h>
 #include "config.h"
 #include "network.h"
 #include "sensors.h"
 #include "actuators.h"
 #include "coop_webserver.h"

 
 unsigned long lastSensorUpdate = 0;
 unsigned long lastSyncTime = 0;
 
 void setup() {
   Serial.begin(115200);
   delay(1000);
   Serial.println("OpenCoopControl starting...");
   
   if (!SPIFFS.begin(true)) {
     Serial.println("ERROR: Failed to mount SPIFFS");
     delay(3000);
     ESP.restart();
   }
   Serial.println("SPIFFS mounted successfully");
   
   pinMode(LED_PIN, OUTPUT);
   digitalWrite(LED_PIN, HIGH);
   
   initializeSensors();
   initializeActuators();
   
   if (!initializeNetwork()) {
     Serial.println("Network initialization failed");
   }
   
   initializeWebServer();
   
   updateSensorReadings();
   
   digitalWrite(LED_PIN, LOW);
   Serial.println("Initialization complete");
 }
 
 void loop() {
   handleWebServerClient();
   
   if (millis() - lastSensorUpdate > SENSOR_UPDATE_INTERVAL) {
     updateSensorReadings();
     lastSensorUpdate = millis();
   }
   
   checkManualControls();
   
   time_t now = time(nullptr);
   processAutomation(hour(now), minute(now));
   
   if (WiFi.status() == WL_CONNECTED && millis() - lastSyncTime > 3600000) {
     if (syncWithServer()) {
       SensorReadings readings = getCurrentReadings();
       sendTelemetry(readings.temperature, readings.lightLevel, 
                     readings.waterLevel, isDoorOpen());
     }
     lastSyncTime = millis();
   }
 }