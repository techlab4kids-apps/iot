#ifndef _wifi_configuration_
#define _wifi_configuration_

#include <ESP8266WiFi.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

// External secrets (optional - if secrets.h exists, it overrides defaults)
#ifdef INCLUDE_SECRETS
#include "secrets.h"
#endif

// WiFi Credentials
// Priority: secrets.h > build flags > WiFiManager captive portal
#ifndef WIFI_SSID
  #define WIFI_SSID ""
#endif

#ifndef WIFI_PASSWORD
  #define WIFI_PASSWORD ""
#endif

void impostaLaConnessioneWiFi();
void initWiFi();

void ricconnettiIlWifi();
void reconnectWifi();

void verificaSeConnessoEtRiconnettiSeNecessario();
void checkWifiConnectionAndRecconnectIfNecessary();

#endif //_wifi_configuration_
