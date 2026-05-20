/**
 * Tl4k-Seed-Config.h - Configuration struct and EEPROM storage for seed provisioning.
 *
 * Stores WiFi + MQTT credentials in ESP8266 internal flash (EEPROM emulation).
 * Each board checks this on boot: if configured → normal mode, if not → seed slave mode.
 */

#ifndef TL4K_SEED_CONFIG_H
#define TL4K_SEED_CONFIG_H

#include <Arduino.h>
#include <EEPROM.h>

// EEPROM layout (ESP8266 has 4KB emulated EEPROM)
#define EEPROM_SIZE             512
#define EEPROM_MAGIC_OFFSET     0
#define EEPROM_MAGIC_VALUE      0x544C344B  // "TL4K" in hex
#define EEPROM_VERSION_OFFSET   4
#define EEPROM_VERSION          1
#define EEPROM_CONFIG_OFFSET    8

// Seed AP naming
#define SEED_AP_PREFIX          "TL4K-SEED-"
#define SEED_AP_PASSWORD         "seedconfig"  // Fixed password for seed hotspot
#define SEED_WEB_SERVER_PORT    80

// Timeout for seed HTTP request (ms)
#define SEED_HTTP_TIMEOUT_MS    5000

// Maximum length for credential fields
#define MAX_SSID_LEN            33
#define MAX_PASS_LEN            65
#define MAX_HOST_LEN            65
#define MAX_TOKEN_LEN           65

/**
 * TL4K board configuration stored in EEPROM.
 */
typedef struct {
  char wifiSsid[MAX_SSID_LEN];
  char wifiPassword[MAX_PASS_LEN];
  char mqttBroker[MAX_HOST_LEN];
  int  mqttPort;
  char mqttUser[MAX_HOST_LEN];   // reuse MAX_HOST_LEN for username
  char mqttPass[MAX_TOKEN_LEN];
  bool isConfigured;             // true if this board has valid credentials
} Tl4kConfig;

// Global config instance (populated on boot)
extern Tl4kConfig tl4kConfig;

/**
 * Initialize EEPROM and load saved configuration.
 * Returns true if valid config was loaded, false otherwise.
 */
bool seedConfigLoad();

/**
 * Save current configuration to EEPROM.
 * Returns true on success.
 */
bool seedConfigSave(const Tl4kConfig &config);

/**
 * Mark the board as unconfigured (forces seed mode on next boot).
 */
void seedConfigReset();

/**
 * Check if the board is already configured.
 */
bool seedConfigIsConfigured();

/**
 * Get a human-readable description of the board state.
 */
const char* seedConfigStateString();

#endif // TL4K_SEED_CONFIG_H
