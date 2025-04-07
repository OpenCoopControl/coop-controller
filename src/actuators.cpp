 #include <Arduino.h>
 #include "actuators.h"
 #include <ArduinoJson.h>
 #include <TimeLib.h>
 #include <FS.h>
 #include "SPIFFS.h" 

 using File = fs::File; 
 Servo doorServo;
 ActuatorSettings settings;
 
 bool doorOpen = false;
 bool feederActive = false;
 String lastDoorChange = "--";
 String lastFeedTime = "--";
 unsigned long feederStartTime = 0;
 unsigned long lastButtonCheck = 0;
 
 void initializeActuators() {
   Serial.println("Initializing actuators...");
   
   if (!SPIFFS.begin(true)) {
       Serial.println("SPIFFS initialization failed!");
   } else {
       Serial.println("SPIFFS initialized successfully");
   }
   
   pinMode(FEEDER_MOTOR_PIN, OUTPUT);
   pinMode(MANUAL_DOOR_BUTTON, INPUT_PULLUP);
   
   doorServo.attach(DOOR_SERVO_PIN);
   
   if (!loadActuatorSettings()) {
     setDefaultActuatorSettings();
   }
   
   closeDoor();
   
   Serial.println("Actuators initialized");
 }
 
 void openDoor() {
   Serial.println("Opening coop door");
   doorServo.write(DOOR_OPEN_ANGLE);
   doorOpen = true;
   
   char timeStr[20];
   sprintf(timeStr, "%02d:%02d:%02d", hour(), minute(), second());
   lastDoorChange = String(timeStr);
 }
 
 void closeDoor() {
   Serial.println("Closing coop door");
   doorServo.write(DOOR_CLOSED_ANGLE);
   doorOpen = false;
   
   char timeStr[20];
   sprintf(timeStr, "%02d:%02d:%02d", hour(), minute(), second());
   lastDoorChange = String(timeStr);
 }
 
 bool isDoorOpen() {
   return doorOpen;
 }
 
 String getLastDoorActionTime() {
   return lastDoorChange;
 }
 
 void activateFeeder(int durationSeconds) {
   int feedDuration = (durationSeconds > 0) ? 
                      durationSeconds : 
                      settings.feedAmount;
   
   Serial.print("Activating feeder for ");
   Serial.print(feedDuration);
   Serial.println(" seconds");
   
   feederActive = true;
   digitalWrite(FEEDER_MOTOR_PIN, HIGH);
   
   feederStartTime = millis();
   
   char timeStr[20];
   sprintf(timeStr, "%02d:%02d:%02d", hour(), minute(), second());
   lastFeedTime = String(timeStr);
 }
 
 bool isFeederActive() {
   if (feederActive) {
     unsigned long elapsedTime = millis() - feederStartTime;
     if (elapsedTime >= (settings.feedAmount * 1000)) {
       digitalWrite(FEEDER_MOTOR_PIN, LOW);
       feederActive = false;
     }
   }
   
   return feederActive;
 }
 
 String getLastFeedTime() {
   return lastFeedTime;
 }
 
 void checkManualControls() {
   if (millis() - lastButtonCheck < BUTTON_DEBOUNCE_DELAY) {
     return;
   }
   
   if (digitalRead(MANUAL_DOOR_BUTTON) == LOW) {
     if (doorOpen) {
       closeDoor();
     } else {
       openDoor();
     }
     
     lastButtonCheck = millis();
   }
 }
 
 void processAutomation(int currentHour, int currentMinute) {
   
   if (settings.doorAutoMode) {
     if (currentHour == settings.doorOpenHour && currentMinute == 0 && !doorOpen) {
       openDoor();
     } else if (currentHour == settings.doorCloseHour && currentMinute == 0 && doorOpen) {
       closeDoor();
     }
   }
   
   if (settings.feederAutoMode && !feederActive) {
     if ((currentHour == settings.feedHour1 || currentHour == settings.feedHour2) && 
         currentMinute == 0) {
       activateFeeder();
     }
   }
   
   isFeederActive();
 }
 
 bool loadActuatorSettings() {
     if (!SPIFFS.begin(true)) {
            Serial.println("SPIFFS Mount Failed");
            return false;
          }
     if (!SPIFFS.exists(SETTINGS_FILE)) {
            Serial.println("Settings file does not exist");
            return false;
          }
   
     fs::File file = SPIFFS.open(SETTINGS_FILE, "r");
     if (!file) {
          return false;
      }
   StaticJsonDocument<512> doc;
   auto error = deserializeJson(doc, file);
   file.close();
   
   if (error) {
     Serial.println("Failed to parse settings file");
     return false;
   }
   
   settings.doorAutoMode = doc["doorAutoMode"] | DEFAULT_DOOR_AUTO_MODE;
   settings.doorOpenHour = doc["doorOpenHour"] | DEFAULT_DOOR_OPEN_HOUR;
   settings.doorCloseHour = doc["doorCloseHour"] | DEFAULT_DOOR_CLOSE_HOUR;
   settings.lightThreshold = doc["lightThreshold"] | DEFAULT_LIGHT_THRESHOLD;
   settings.feederAutoMode = doc["feederAutoMode"] | DEFAULT_FEEDER_AUTO_MODE;
   settings.feedHour1 = doc["feedHour1"] | DEFAULT_FEED_HOUR_1;
   settings.feedHour2 = doc["feedHour2"] | DEFAULT_FEED_HOUR_2;
   settings.feedAmount = doc["feedAmount"] | DEFAULT_FEED_AMOUNT;
   
   Serial.println("Settings loaded successfully");
   return true;
 }
 
 bool saveActuatorSettings() {
   JsonDocument doc;
   
   doc["doorAutoMode"] = settings.doorAutoMode;
   doc["doorOpenHour"] = settings.doorOpenHour;
   doc["doorCloseHour"] = settings.doorCloseHour;
   doc["lightThreshold"] = settings.lightThreshold;
   doc["feederAutoMode"] = settings.feederAutoMode;
   doc["feedHour1"] = settings.feedHour1;
   doc["feedHour2"] = settings.feedHour2;
   doc["feedAmount"] = settings.feedAmount;
   
   fs::File file = SPIFFS.open(SETTINGS_FILE, "w");
   if (!file) {
     Serial.println("Failed to open settings file for writing");
     return false;
   }
   
   if (serializeJson(doc, file) == 0) {
     Serial.println("Failed to write settings to file");
     file.close();
     return false;
   }
   
   file.close();
   Serial.println("Settings saved successfully");
   return true;
 }
 
 void setDefaultActuatorSettings() {
   Serial.println("Setting default actuator settings");
   
   settings.doorAutoMode = DEFAULT_DOOR_AUTO_MODE;
   settings.doorOpenHour = DEFAULT_DOOR_OPEN_HOUR;
   settings.doorCloseHour = DEFAULT_DOOR_CLOSE_HOUR;
   settings.lightThreshold = DEFAULT_LIGHT_THRESHOLD;
   settings.feederAutoMode = DEFAULT_FEEDER_AUTO_MODE;
   settings.feedHour1 = DEFAULT_FEED_HOUR_1;
   settings.feedHour2 = DEFAULT_FEED_HOUR_2;
   settings.feedAmount = DEFAULT_FEED_AMOUNT;
   
   saveActuatorSettings();
 }
 
 ActuatorSettings getActuatorSettings() {
   return settings;
 }
 
 void updateActuatorSettings(ActuatorSettings newSettings) {
   settings = newSettings;
   saveActuatorSettings();
 }