/**
 * Tl4k-Seed-Config.cpp - EEPROM storage implementation for seed provisioning.
 */

#include "Tl4k-Seed-Config.h"

// Include secrets for default MQTT values if available
#ifdef INCLUDE_SECRETS
#include "secrets.h"
#endif

// Global config instance
Tl4kConfig tl4kConfig;

bool seedConfigLoad() {
  EEPROM.begin(EEPROM_SIZE);

  // Read magic word
  uint32_t magic = 0;
  for (int i = 0; i < 4; i++) {
    magic |= ((uint32_t)EEPROM.read(EEPROM_MAGIC_OFFSET + i)) << (8 * i);
  }

  if (magic != EEPROM_MAGIC_VALUE) {
    EEPROM.end();
    Serial.println("[SEED] EEPROM: no valid config found (magic mismatch)");
    memset(&tl4kConfig, 0, sizeof(Tl4kConfig));
    return false;
  }

  // Read version
  uint8_t version = EEPROM.read(EEPROM_VERSION_OFFSET);
  if (version != EEPROM_VERSION) {
    EEPROM.end();
    Serial.printf("[SEED] EEPROM: unsupported config version %d (expected %d)\n",
                  version, EEPROM_VERSION);
    memset(&tl4kConfig, 0, sizeof(Tl4kConfig));
    return false;
  }

  // Read config struct
  EEPROM.get(EEPROM_CONFIG_OFFSET, tl4kConfig);
  EEPROM.end();

  Serial.println("[SEED] EEPROM: valid config loaded");
  Serial.printf("[SEED]   WiFi SSID:     %s\n", tl4kConfig.wifiSsid);
  Serial.printf("[SEED]   MQTT Broker:   %s:%d\n", tl4kConfig.mqttBroker, tl4kConfig.mqttPort);
  Serial.printf("[SEED]   Configured:    %s\n", tl4kConfig.isConfigured ? "YES" : "NO");

  return tl4kConfig.isConfigured;
}

bool seedConfigSave(const Tl4kConfig &config) {
  EEPROM.begin(EEPROM_SIZE);

  // Write magic
  uint32_t magic = EEPROM_MAGIC_VALUE;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_MAGIC_OFFSET + i, (magic >> (8 * i)) & 0xFF);
  }

  // Write version
  EEPROM.write(EEPROM_VERSION_OFFSET, EEPROM_VERSION);

  // Write config struct
  EEPROM.put(EEPROM_CONFIG_OFFSET, config);

  bool ok = EEPROM.commit();
  EEPROM.end();

  if (ok) {
    Serial.println("[SEED] EEPROM: config saved successfully");
  } else {
    Serial.println("[SEED] EEPROM: FAILED to save config");
  }

  return ok;
}

void seedConfigReset() {
  EEPROM.begin(EEPROM_SIZE);

  // Write magic but mark as unconfigured
  uint32_t magic = EEPROM_MAGIC_VALUE;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(EEPROM_MAGIC_OFFSET + i, (magic >> (8 * i)) & 0xFF);
  }
  EEPROM.write(EEPROM_VERSION_OFFSET, EEPROM_VERSION);

  Tl4kConfig emptyConfig;
  memset(&emptyConfig, 0, sizeof(Tl4kConfig));
  emptyConfig.isConfigured = false;
  #ifdef MQTT_BROKER_PORT
    emptyConfig.mqttPort = MQTT_BROKER_PORT;
  #else
    emptyConfig.mqttPort = 443;  // default WSS port
  #endif

  EEPROM.put(EEPROM_CONFIG_OFFSET, emptyConfig);

  bool ok = EEPROM.commit();
  EEPROM.end();

  Serial.printf("[SEED] EEPROM: board marked as UNCONFIGURED (%s)\n",
                ok ? "saved" : "failed");
}

bool seedConfigIsConfigured() {
  return tl4kConfig.isConfigured;
}

const char* seedConfigStateString() {
  if (tl4kConfig.isConfigured) {
    return "CONFIGURED - Normal mode";
  }
  return "NOT_CONFIGURED - Seed mode required";
}
