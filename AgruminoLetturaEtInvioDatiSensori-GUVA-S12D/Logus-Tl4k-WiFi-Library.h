#ifndef _wifi_configuration_
#define _wifi_configuration_

#include <ESP8266WiFi.h>
#include <Fetch.h>

// Nome della rete WiFi(SID)
#define WIFI_AP_NAME "techlab4kids"

// WiFi password
// #define WIFI_PASSWORD "12344321"
#define WIFI_PASSWORD "techlab4kids"

// the Wifi radio's status
// extern int status;

// Initialize client
extern WiFiClient espClient;

void impostaLaConnessioneWiFi();
void initWiFi();

void ricconnettiIlWifi();
void reconnectWifi();

void verificaSeConnessoEtRiconnettiSeNecessario();
void checkWifiConnectionAndRecconnectIfNecessary();

void testHttpConnection();

#endif //_wifi_configuration_
