 #ifndef SENSORS_H
 #define SENSORS_H
 
 #include <Arduino.h>
 #include "config.h"
 
 struct SensorReadings {
     float temperature;  // Temperature in Celsius
     int lightLevel;     // Light level (0-100%)
     int waterLevel;     // Water level (0-100%)
     unsigned long timestamp; // Last update timestamp
 };
 
 void initializeSensors();
 void updateSensorReadings();
 SensorReadings getCurrentReadings();
 float getTemperature();
 int getLightLevel();
 int getWaterLevel();
 bool isDaytime();
 
 #endif