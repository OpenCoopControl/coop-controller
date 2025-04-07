#ifndef COOP_WEBSERVER_H
#define COOP_WEBSERVER_H

#include <Arduino.h>
#include <WebServer.h> 
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "sensors.h"
#include "actuators.h"

extern WebServer server;

void initializeWebServer();
void handleWebServerClient();
void setupWebServerRoutes();
void handleRoot();
void handleNotFound();
void handleGetState();
void handleGetSettings();
void handleUpdateSettings();
void handleDoorOpen();
void handleDoorClose();
void handleFeederActivate();

#endif