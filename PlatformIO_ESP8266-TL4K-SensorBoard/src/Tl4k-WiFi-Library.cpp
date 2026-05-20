
#include "Tl4k-WiFi-Library.h"
#include "Tl4k-Seed-Config.h"
#include "Tl4k-Seed-Library.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

int status = WL_IDLE_STATUS;

WiFiManager wifiManager;

const char* menu_items[] = {"wifi", "param"};

void impostaLaConnessioneWiFi(){
   initWiFi();
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

/**
 * Start mDNS advertiser if the board is configured.
 * Called after WiFi is connected and config is loaded.
 */
void startMdnsAdvertiserIfConfigured() {
  if (seedConfigIsConfigured() && strlen(tl4kConfig.mqttBroker) > 0) {
    seedStartMdnsAdvertiser();
  }
}

void saveWifiToEEPROM(const char* ssid, const char* password) {
  Serial.println("[WIFI] Saving credentials to EEPROM...");
  strncpy(tl4kConfig.wifiSsid, ssid, MAX_SSID_LEN - 1);
  strncpy(tl4kConfig.wifiPassword, password, MAX_PASS_LEN - 1);
  tl4kConfig.isConfigured = true;
  seedConfigSave(tl4kConfig);
}

void initWiFi()
{
  // If already configured via seed, use saved credentials directly
  if (seedConfigIsConfigured() && strlen(tl4kConfig.wifiSsid) > 0) {
    Serial.printf("[WIFI] Using saved WiFi config: %s\n", tl4kConfig.wifiSsid);
    WiFi.begin(tl4kConfig.wifiSsid, tl4kConfig.wifiPassword);

    unsigned long connectStart = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - connectStart < 20000) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
      Serial.printf("[WIFI] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
      startMdnsAdvertiserIfConfigured();
      return;
    }

    Serial.println("[WIFI] Saved credentials failed, falling back to WiFiManager...");
  }

  // Fallback to WiFiManager captive portal
  wifiManager.setDebugOutput(true);

  wifiManager.setMenu(menu_items, 2);

  wifiManager.setConfigPortalTimeout(180);
  wifiManager.setConnectTimeout(20);

  WiFiManagerParameter custom_mqtt_server_name("mqtt_server_name", "mqtt server name", "mqtt.local", 40);
  wifiManager.addParameter(&custom_mqtt_server_name);

  wifiManager.setAPCallback(configModeCallback);

  // Save credentials to EEPROM after WiFiManager connects
  wifiManager.setSaveConfigCallback([&]() {
    Serial.println("[WIFI] WiFiManager save callback triggered");
    String ssid = WiFi.SSID();
    String pass = WiFi.psk();
    Serial.printf("[WIFI] Connected to: %s\n", ssid.c_str());
    strncpy(tl4kConfig.wifiSsid, ssid.c_str(), MAX_SSID_LEN - 1);
    strncpy(tl4kConfig.wifiPassword, pass.c_str(), MAX_PASS_LEN - 1);
    strncpy(tl4kConfig.mqttBroker, custom_mqtt_server_name.getValue(), MAX_HOST_LEN - 1);
    tl4kConfig.mqttPort = 443;
    tl4kConfig.isConfigured = true;
    seedConfigSave(tl4kConfig);
    Serial.printf("[WIFI] MQTT broker saved: %s:443/mqtt\n", tl4kConfig.mqttBroker);
  });

  Serial.println("Connessione all'Access Point...");
  wifiManager.autoConnect();

  Serial.printf("[WIFI] MQTT Server: %s:443/mqtt\n", custom_mqtt_server_name.getValue());

  Serial.println();

  startMdnsAdvertiserIfConfigured();
}

void reconnectWifi()
{
  status = WiFi.status();
  Serial.println("[WIFI] Tentativo di riconnessione all'Access Point in corso");
  if (status != WL_CONNECTED)
  {
    if (strlen(tl4kConfig.wifiSsid) > 0) {
      WiFi.begin(tl4kConfig.wifiSsid, tl4kConfig.wifiPassword);
      Serial.printf("[WIFI] Reconnecting to: %s\n", tl4kConfig.wifiSsid);
    } else {
      Serial.println("[WIFI] No saved SSID - falling back to WiFiManager auto-connect");
      wifiManager.autoConnect();
      return;
    }
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println("[WIFI] Connessione completata!");

    Serial.printf("\n[WIFI] L'indirizzo IP locale è %s\n", WiFi.localIP().toString().c_str());
    Serial.println();
  }
}

void verificaSeConnessoEtRiconnettiSeNecessario(){
  checkWifiConnectionAndRecconnectIfNecessary();
}

void checkWifiConnectionAndRecconnectIfNecessary()
{
  // Reconnect to WiFi, if needed
  bool isWifiConnected = WiFi.status() != WL_CONNECTED;
  if (isWifiConnected)
  {
    Serial.println("[WIFI] Connecting to WiFi");
    reconnectWifi();
    return;
  }
}
