 #ifndef ACTUATORS_H
 #define ACTUATORS_H
 
 #include <Arduino.h>
 #include <ESP32Servo.h>
 #include "config.h"
 
 void initializeActuators();
 void openDoor();
 void closeDoor();
 bool isDoorOpen();
 String getLastDoorActionTime();
 void activateFeeder(int durationSeconds = 0);
 bool isFeederActive();
 String getLastFeedTime();
 void checkManualControls();
 void processAutomation(int currentHour, int currentMinute);
 
 struct ActuatorSettings {
     bool doorAutoMode;
     int doorOpenHour;
     int doorCloseHour;
     int lightThreshold;
     bool feederAutoMode;
     int feedHour1;
     int feedHour2;
     int feedAmount; // in seconds
 };
 
 bool loadActuatorSettings();
 bool saveActuatorSettings();
 void setDefaultActuatorSettings();
 ActuatorSettings getActuatorSettings();
 void updateActuatorSettings(ActuatorSettings newSettings);
 
 #endif