 #include "sensors.h"
 #include "actuators.h"
 
 SensorReadings currentReadings;
 
 void initializeSensors() {
   Serial.println("Initializing sensors...");
   
   pinMode(TEMP_SENSOR_PIN, INPUT);
   pinMode(LIGHT_SENSOR_PIN, INPUT);
   pinMode(WATER_LEVEL_PIN, INPUT);
   
   currentReadings.temperature = 0.0;
   currentReadings.lightLevel = 0;
   currentReadings.waterLevel = 0;
   currentReadings.timestamp = 0;
   
   Serial.println("Sensors initialized");
 }
 
 void updateSensorReadings() {
   int rawTemp = analogRead(TEMP_SENSOR_PIN);
   currentReadings.temperature = (rawTemp / 4095.0) * 3.3 * 100.0;
   
   int rawLight = analogRead(LIGHT_SENSOR_PIN);
   currentReadings.lightLevel = map(rawLight, 0, 4095, 0, 100);
   
   int rawWater = analogRead(WATER_LEVEL_PIN);
   currentReadings.waterLevel = map(rawWater, 0, 4095, 0, 100);
   
   currentReadings.timestamp = millis();
   
   Serial.println("Sensor readings updated:");
   Serial.print("Temperature: "); Serial.print(currentReadings.temperature); Serial.println("°C");
   Serial.print("Light level: "); Serial.print(currentReadings.lightLevel); Serial.println("%");
   Serial.print("Water level: "); Serial.print(currentReadings.waterLevel); Serial.println("%");
 }
 
 SensorReadings getCurrentReadings() {
   return currentReadings;
 }
 
 float getTemperature() {
   return currentReadings.temperature;
 }
 
 int getLightLevel() {
   return currentReadings.lightLevel;
 }
 
 int getWaterLevel() {
   return currentReadings.waterLevel;
 }
 
 bool isDaytime() {
   ActuatorSettings settings = getActuatorSettings();
   
   return currentReadings.lightLevel > settings.lightThreshold;
 }