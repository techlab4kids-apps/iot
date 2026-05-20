/**
 * Tl4k-Seed-Library.cpp - mDNS-based configuration propagation.
 *
 * MASTER: advertises _tl4k._tcp.local via mDNS + serves config via HTTP GET /tl4k-config
 * SLAVE: queries mDNS → gets IP → HTTP GET /tl4k-config → saves to EEPROM → reboots
 */

#include "Tl4k-Seed-Library.h"
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// HTTP server on master to serve config
static ESP8266WebServer* seedConfigServer = nullptr;

static void seedHandleConfigRequest() {
  JsonDocument doc;
  doc["broker_url"]   = tl4kConfig.mqttBroker;
  doc["broker_port"]   = tl4kConfig.mqttPort;
  doc["mqtt_user"]     = tl4kConfig.mqttUser;
  doc["mqtt_pass"]     = tl4kConfig.mqttPass;
  doc["config_version"] = EEPROM_VERSION;

  String json;
  serializeJson(doc, json);

  seedConfigServer->send(200, "application/json", json);
}

void seedStartMdnsAdvertiser() {
  String hostname = "tl4k-" + String(ESP.getChipId(), HEX);

  Serial.printf("[SEED] Starting mDNS advertiser: %s.local\n", hostname.c_str());

  if (!MDNS.begin(hostname.c_str())) {
    Serial.println("[SEED] ERROR: Failed to start mDNS responder");
    return;
  }

  // Add the TL4K service (port 80 = HTTP config server)
  MDNS.addService(SEED_MDNS_SERVICE_NAME, SEED_MDNS_SERVICE_PROTO, SEED_HTTP_SERVER_PORT);

  Serial.println("[SEED] mDNS service advertised:");
  Serial.printf("  broker_url  = %s\n", tl4kConfig.mqttBroker);
  Serial.printf("  broker_port = %d\n", tl4kConfig.mqttPort);
  Serial.printf("  device_id   = %s\n", String(ESP.getChipId(), HEX).c_str());

  // Start HTTP server to serve config
  if (seedConfigServer == nullptr) {
    seedConfigServer = new ESP8266WebServer(SEED_HTTP_SERVER_PORT);
    seedConfigServer->on(SEED_HTTP_CONFIG_PATH, HTTP_GET, seedHandleConfigRequest);
    seedConfigServer->begin();
    Serial.printf("[SEED] HTTP config server started on port %d%s\n",
                  SEED_HTTP_SERVER_PORT, SEED_HTTP_CONFIG_PATH);
  }
}

void seedMdnsAdvertiserLoop() {
  MDNS.update();
  if (seedConfigServer != nullptr) {
    seedConfigServer->handleClient();
  }
}

// ============================================================================
// SLAVE MODE - mDNS scanner + HTTP config fetch
// ============================================================================

static bool fetchConfigFromMaster(IPAddress masterIP, Tl4kConfig &config) {
  Serial.printf("[SEED] Fetching config from %s:%d%s\n",
                masterIP.toString().c_str(), SEED_HTTP_SERVER_PORT, SEED_HTTP_CONFIG_PATH);

  WiFiClient client;
  if (!client.connect(masterIP, SEED_HTTP_SERVER_PORT)) {
    Serial.println("[SEED] ERROR: Cannot connect to master HTTP server");
    return false;
  }

  client.print("GET ");
  client.print(SEED_HTTP_CONFIG_PATH);
  client.println(" HTTP/1.0");
  client.print("Host: ");
  client.println(masterIP.toString());
  client.println("Connection: close");
  client.println();

  // Wait for response
  unsigned long timeout = millis() + 5000;
  while (!client.available() && millis() < timeout) {
    delay(10);
  }

  if (!client.available()) {
    Serial.println("[SEED] ERROR: No response from master");
    client.stop();
    return false;
  }

  // Skip HTTP headers
  while (client.available()) {
    String line = client.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) break;
  }

  // Read JSON body
  String jsonBody;
  while (client.available()) {
    jsonBody += (char)client.read();
  }
  client.stop();

  if (jsonBody.length() == 0) {
    Serial.println("[SEED] ERROR: Empty response");
    return false;
  }

  Serial.printf("[SEED] Received config: %s\n", jsonBody.c_str());

  // Parse JSON
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, jsonBody);
  if (err) {
    Serial.printf("[SEED] ERROR: JSON parse failed: %s\n", err.c_str());
    return false;
  }

  // Validate required fields
  if (!doc.containsKey("broker_url") || !doc.containsKey("broker_port")) {
    Serial.println("[SEED] ERROR: Missing required fields");
    return false;
  }

  // Populate config
  memset(&config, 0, sizeof(Tl4kConfig));

  const char* brokerUrl = doc["broker_url"] | "";
  const char* mqttUser = doc["mqtt_user"] | "";
  const char* mqttPass = doc["mqtt_pass"] | "";
  int mqttPort = doc["broker_port"] | 8883;

  strncpy(config.mqttBroker, brokerUrl, MAX_HOST_LEN - 1);
  config.mqttPort = mqttPort;
  strncpy(config.mqttUser, mqttUser, MAX_HOST_LEN - 1);
  strncpy(config.mqttPass, mqttPass, MAX_TOKEN_LEN - 1);
  config.isConfigured = true;

  return true;
}

bool seedRunMdnsSlave(int timeoutSeconds) {
  Serial.println("[SEED] ===========================================");
  Serial.println("[SEED] Starting mDNS SLAVE mode");
  Serial.println("[SEED] Looking for TL4K config via mDNS...");
  Serial.println("[SEED] ===========================================");

  unsigned long startTime = millis();
  unsigned long scanInterval = 2000;
  unsigned long lastScanTime = 0;

  while (millis() - startTime < (unsigned long)timeoutSeconds * 1000) {
    if (millis() - lastScanTime > scanInterval) {
      lastScanTime = millis();

      Serial.println("[SEED] Scanning mDNS for mqtt.local ...");

      int n = MDNS.queryService(SEED_MDNS_SERVICE_NAME, SEED_MDNS_SERVICE_PROTO);

      if (n > 0) {
        Serial.printf("[SEED] Found %d TL4K service(s)!\n", n);

        // Try each discovered service
        for (int i = 0; i < n; i++) {
          IPAddress ip = MDNS.IP(i);
          Serial.printf("[SEED] Trying service %d at %s:%d\n", i, ip.toString().c_str(), MDNS.port(i));

          Tl4kConfig receivedConfig;
          if (fetchConfigFromMaster(ip, receivedConfig)) {
            // Save to EEPROM
            if (!seedConfigSave(receivedConfig)) {
              Serial.println("[SEED] ERROR: Failed to save config");
              continue;
            }

            Serial.println("[SEED] ===========================================");
            Serial.println("[SEED] Config received and saved!");
            Serial.printf("[SEED]   broker_url  = %s\n", receivedConfig.mqttBroker);
            Serial.printf("[SEED]   broker_port = %d\n", receivedConfig.mqttPort);
            Serial.printf("[SEED]   mqtt_user   = %s\n", receivedConfig.mqttUser);
            Serial.println("[SEED] Rebooting to apply configuration...");
            Serial.println("[SEED] ===========================================");

            delay(2000);
            ESP.restart();
            return true;  // never reached
          }
        }

        Serial.println("[SEED] All discovered services failed, retrying...");
      } else {
        Serial.println("[SEED] No TL4K services found, retrying...");
      }
    }

    yield();
    delay(100);
  }

  Serial.println("[SEED] ===========================================");
  Serial.println("[SEED] Timeout: no mDNS config found");
  Serial.println("[SEED] Using fallback defaults from secrets.h");
  Serial.println("[SEED] ===========================================");

  // Fallback: use hardcoded defaults
  memset(&tl4kConfig, 0, sizeof(Tl4kConfig));
  #ifdef MQTT_BROKER_ADDR
    strncpy(tl4kConfig.mqttBroker, MQTT_BROKER_ADDR, MAX_HOST_LEN - 1);
  #endif
  #ifdef MQTT_BROKER_PORT
    tl4kConfig.mqttPort = MQTT_BROKER_PORT;
  #else
    tl4kConfig.mqttPort = 443;
  #endif
  #ifdef MQTT_BROKER_USER
    strncpy(tl4kConfig.mqttUser, MQTT_BROKER_USER, MAX_HOST_LEN - 1);
  #endif
  #ifdef MQTT_BROKER_PASS
    strncpy(tl4kConfig.mqttPass, MQTT_BROKER_PASS, MAX_TOKEN_LEN - 1);
  #endif
  tl4kConfig.isConfigured = true;

  if (!seedConfigSave(tl4kConfig)) {
    Serial.println("[SEED] ERROR: Failed to save fallback config");
    return false;
  }

  Serial.println("[SEED] Fallback config saved, rebooting...");
  delay(2000);
  ESP.restart();
  return true;
}

// ============================================================================
// UTILITIES
// ============================================================================

bool seedIsButtonPressedOnBoot() {
  pinMode(SEED_ENABLE_PIN, INPUT_PULLUP);
  delay(50);  // settle
  return (digitalRead(SEED_ENABLE_PIN) == LOW);
}

void seedPrintStatus() {
  Serial.println("==========================================");
  Serial.println("  TL4K Board Status");
  Serial.println("==========================================");
  Serial.printf("  Mode:         %s\n", seedConfigStateString());
  Serial.printf("  Chip ID:      %s\n", String(ESP.getChipId(), HEX).c_str());
  Serial.printf("  Free Heap:    %d bytes\n", ESP.getFreeHeap());

  if (seedConfigIsConfigured()) {
    Serial.printf("  WiFi SSID:    %s\n", tl4kConfig.wifiSsid);
    Serial.printf("  MQTT Broker:  %s:%d\n", tl4kConfig.mqttBroker, tl4kConfig.mqttPort);
    Serial.printf("  mDNS Service: _%s._%s.local\n", SEED_MDNS_SERVICE_NAME, SEED_MDNS_SERVICE_PROTO);
  }
  Serial.println("==========================================");
}
