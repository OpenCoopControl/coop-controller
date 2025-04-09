#ifndef COOP_CONFIG_H
#define COOP_CONFIG_H

#include "version.h"

#define TEMP_SENSOR_PIN 32    // Temperature sensor (analog)
#define LIGHT_SENSOR_PIN 33   // Light sensor (analog)
#define DOOR_SERVO_PIN 13     // Servo for coop door
#define FEEDER_MOTOR_PIN 12   // Motor for feeder
#define WATER_LEVEL_PIN 34    // Water level sensor (analog)
#define MANUAL_DOOR_BUTTON 14 // Manual door control button
#define LED_PIN 2             // Built-in LED for status

// Door servo angles
#define DOOR_CLOSED_ANGLE 0
#define DOOR_OPEN_ANGLE 90

// Default settings (can be overridden through the web interface)
#define DEFAULT_DOOR_AUTO_MODE true
#define DEFAULT_DOOR_OPEN_HOUR 6  // 6 AM
#define DEFAULT_DOOR_CLOSE_HOUR 21 // 9 PM for our American friends
#define DEFAULT_LIGHT_THRESHOLD 30
#define DEFAULT_FEEDER_AUTO_MODE true
#define DEFAULT_FEED_HOUR_1 6  // 6 AM
#define DEFAULT_FEED_HOUR_2 19 // 7 PM
#define DEFAULT_FEED_AMOUNT 7  // 7 seconds

// Server and JSON settings
#define WEB_SERVER_PORT 80
#define JSON_CAPACITY 512

// Update intervals (milliseconds)
#define SENSOR_UPDATE_INTERVAL 5000
#define BUTTON_DEBOUNCE_DELAY 500

// Device configuration
#define CONFIG_FILE "/config.json"
#define SETTINGS_FILE "/settings.json"
#define FACTORY_RESET_HOLD_TIME 10000 // 10 seconds

// AP Configuration
#define AP_NAME_PREFIX "OpenCoop-"
#define AP_TIMEOUT 180 // seconds

#endif