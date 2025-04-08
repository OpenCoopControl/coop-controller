#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>

#include "config.h"
#include "sensors.h"
#include "actuators.h"
#include "network.h"
#include "coop_webserver.h"
#include "version.h"
 
using fs::File;
 
WebServer server(WEB_SERVER_PORT);

void initializeWebServer() {
  Serial.println("Initializing web server...");
  
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed in webserver");
  }
  
  setupWebServerRoutes();
  
  server.begin();
  Serial.println("Web server started on port " + String(WEB_SERVER_PORT));
}
 
void handleWebServerClient() {
  server.handleClient();
}

void handleRoot();
void handleNotFound();
void handleGetState();
void handleGetSettings();
void handleUpdateSettings();
void handleDoorOpen();
void handleDoorClose();
void handleFeederActivate();
void handleGetVersion();
void handleVersionJs();
 
void setupWebServerRoutes() {
  server.on("/", HTTP_GET, handleRoot);
   
  server.on("/api/state", HTTP_GET, handleGetState);
   
  server.on("/api/settings", HTTP_GET, handleGetSettings);
  server.on("/api/settings", HTTP_POST, handleUpdateSettings);
   
  server.on("/api/door/open", HTTP_POST, handleDoorOpen);
  server.on("/api/door/close", HTTP_POST, handleDoorClose);
  server.on("/api/feeder/activate", HTTP_POST, handleFeederActivate);
  
  server.on("/api/version", HTTP_GET, handleGetVersion);
  
  server.on("/version.js", HTTP_GET, handleVersionJs);
   
  server.onNotFound(handleNotFound);
}

void handleGetVersion() {
  JsonDocument doc;
  
  doc["version"] = FIRMWARE_VERSION;
  doc["name"] = FIRMWARE_NAME;
  doc["buildTimestamp"] = BUILD_TIMESTAMP;
  doc["author"] = FIRMWARE_AUTHOR;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleVersionJs() {
  server.send(200, "application/javascript", VERSION_STRING);
}
 
void handleRoot() {
  if (SPIFFS.exists("/index.html")) {
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
    file.close();
  } else {
    String html = "<html><head><title>" + getCoopName() + "</title>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>body{font-family:Arial;margin:20px;} .card{background:#f0f0f0;padding:15px;margin:10px 0;border-radius:5px;}</style>";
    html += "</head><body>";
    html += "<h1>" + getCoopName() + "</h1>";
    html += "<p>Device ID: " + getDeviceId() + "</p>";
    
    SensorReadings readings = getCurrentReadings();
    html += "<div class='card'>";
    html += "<h2>Sensor Readings</h2>";
    html += "<p>Temperature: " + String(readings.temperature) + "°C</p>";
    html += "<p>Light Level: " + String(readings.lightLevel) + "%</p>";
    html += "<p>Water Level: " + String(readings.waterLevel) + "%</p>";
    html += "</div>";
    
    html += "<div class='card'>";
    html += "<h2>Door Status: " + String(isDoorOpen() ? "Open" : "Closed") + "</h2>";
    html += "<p>Last changed: " + getLastDoorActionTime() + "</p>";
    html += "<form action='/api/door/open' method='post'><button>Open Door</button></form>";
    html += "<form action='/api/door/close' method='post'><button>Close Door</button></form>";
    html += "</div>";
    
    html += "<div class='card'>";
    html += "<h2>Feeder Status: " + String(isFeederActive() ? "Active" : "Inactive") + "</h2>";
    html += "<p>Last fed: " + getLastFeedTime() + "</p>";
    html += "<form action='/api/feeder/activate' method='post'><button>Activate Feeder</button></form>";
    html += "</div>";
    
    html += "<div style='margin-top:20px;font-size:12px;text-align:center;color:#666;'>";
    html += String(FIRMWARE_NAME) + " v" + String(FIRMWARE_VERSION) + " | " + String(BUILD_TIMESTAMP);
    html += "</div>";
    
    html += "</body></html>";
    server.send(200, "text/html", html);
  }
}
 
void handleNotFound() {
  String path = server.uri();
  if (path.endsWith("/")) path += "index.html";
  if (SPIFFS.exists(path)) {
    String contentType = "text/html";
    if (path.endsWith(".css")) contentType = "text/css";
    else if (path.endsWith(".js")) contentType = "application/javascript";
    else if (path.endsWith(".json")) contentType = "application/json";
    
    File file = SPIFFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return;
  }
  
  server.send(404, "text/plain", "File not found");
}
 
void handleGetState() {
  JsonDocument doc;
  SensorReadings readings = getCurrentReadings();
  
  doc["temperature"] = readings.temperature;
  doc["lightLevel"] = readings.lightLevel;
  doc["waterLevel"] = readings.waterLevel;
  
  doc["doorOpen"] = isDoorOpen();
  doc["feederActive"] = isFeederActive();
  doc["lastFeedTime"] = getLastFeedTime();
  doc["doorLastChanged"] = getLastDoorActionTime();
  
  doc["deviceId"] = getDeviceId();
  doc["coopName"] = getCoopName();
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}
 
void handleGetSettings() {
  JsonDocument doc;
  ActuatorSettings settings = getActuatorSettings();
  
  doc["doorAutoMode"] = settings.doorAutoMode;
  doc["doorOpenHour"] = settings.doorOpenHour;
  doc["doorCloseHour"] = settings.doorCloseHour;
  doc["lightThreshold"] = settings.lightThreshold;
  doc["feederAutoMode"] = settings.feederAutoMode;
  doc["feedHour1"] = settings.feedHour1;
  doc["feedHour2"] = settings.feedHour2;
  doc["feedAmount"] = settings.feedAmount;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}
 
void handleUpdateSettings() {
  if (server.hasArg("plain")) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    
    if (!error) {
      ActuatorSettings settings = getActuatorSettings();
      
      if (doc["doorAutoMode"].is<bool>()) settings.doorAutoMode = doc["doorAutoMode"].as<bool>();
      if (doc["doorOpenHour"].is<int>()) settings.doorOpenHour = doc["doorOpenHour"].as<int>();
      if (doc["doorCloseHour"].is<int>()) settings.doorCloseHour = doc["doorCloseHour"].as<int>();
      if (doc["lightThreshold"].is<int>()) settings.lightThreshold = doc["lightThreshold"].as<int>();
      if (doc["feederAutoMode"].is<bool>()) settings.feederAutoMode = doc["feederAutoMode"].as<bool>();
      if (doc["feedHour1"].is<int>()) settings.feedHour1 = doc["feedHour1"].as<int>();
      if (doc["feedHour2"].is<int>()) settings.feedHour2 = doc["feedHour2"].as<int>();
      if (doc["feedAmount"].is<int>()) settings.feedAmount = doc["feedAmount"].as<int>();
      
      updateActuatorSettings(settings);
      
      server.send(200, "text/plain", "Settings updated");
    } else {
      server.send(400, "text/plain", "Invalid JSON");
    }
  } else {
    server.send(400, "text/plain", "No data received");
  }
}
 
void handleDoorOpen() {
  openDoor();
  server.send(200, "text/plain", "Door opened");
}
 
void handleDoorClose() {
  closeDoor();
  server.send(200, "text/plain", "Door closed");
}
 
void handleFeederActivate() {
  int duration = 0;
  if (server.hasArg("duration")) {
    duration = server.arg("duration").toInt();
  }
  
  activateFeeder(duration);
  server.send(200, "text/plain", "Feeder activated");
}