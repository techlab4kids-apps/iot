#ifndef _wifi_configuration_
#define _wifi_configuration_

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

// Nome della rete WiFi(SID)
#define WIFI_AP_NAME "TL4K-4G-NET"
//#define WIFI_AP_NAME "procioniopossum"

// WiFi password
// #define WIFI_PASSWORD "12344321"
#define WIFI_PASSWORD "techlab4kids"
//#define WIFI_PASSWORD "tombolina"

// the Wifi radio's status
// extern int status;

// Initialize client

void impostaLaConnessioneWiFi();
void initWiFi();

void ricconnettiIlWifi();
void reconnectWifi();

void verificaSeConnessoEtRiconnettiSeNecessario();
void checkWifiConnectionAndRecconnectIfNecessary();

#endif //_wifi_configuration_
