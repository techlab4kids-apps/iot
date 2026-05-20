/**
 * Tl4k-Seed-Library.h - mDNS-based configuration propagation.
 *
 * MASTER (already configured):
 *   - After WiFi + MQTT connect: advertises mDNS service _tl4k._tcp.local
 *     and serves config JSON via HTTP GET /tl4k-config.
 *   - Also publishes config to MQTT topic tl4k/config/master.
 *
 * SLAVE (not configured):
 *   - Scans mDNS for _tl4k._tcp.local service.
 *   - Connects to discovered IP, fetches config via HTTP GET /tl4k-config.
 *   - Saves to EEPROM, reboots.
 *
 * Fallback: if no mDNS found after timeout, uses hardcoded defaults from secrets.h.
 */

#ifndef TL4K_SEED_LIBRARY_H
#define TL4K_SEED_LIBRARY_H

#include "Arduino.h"
#include "Tl4k-Seed-Config.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

// mDNS service name for TL4K config propagation
#define SEED_MDNS_SERVICE_NAME  "mqtt.local"
#define SEED_MDNS_SERVICE_PROTO "tcp"
#define SEED_MDNS_PORT          80  // dummy port, actual broker port is in TXT

// mDNS scan timeout (seconds)
#define SEED_MDNS_SCAN_TIMEOUT  30

// HTTP endpoint on master for config fetch
#define SEED_HTTP_CONFIG_PATH   "/tl4k-config"
#define SEED_HTTP_SERVER_PORT   80

// Pin used to force seed slave mode on boot (button press)
#ifndef SEED_ENABLE_PIN
#define SEED_ENABLE_PIN D1
#endif

/**
 * Start advertising this board as a config source via mDNS.
 * Call after WiFi + MQTT are connected.
 * Non-blocking.
 */
void seedStartMdnsAdvertiser();

/**
 * Update loop for the mDNS advertiser (keeps MDNS responder alive).
 * Call in loop().
 */
void seedMdnsAdvertiserLoop();

/**
 * Run as a seed SLAVE: scan mDNS for TL4K config, connect to master,
 * fetch config JSON, save to EEPROM, and reboot.
 *
 * BLOCKING: runs until config is received or timeout.
 *
 * @param timeoutSeconds Maximum time to wait (default 30s).
 * @returns true if configuration was received and saved.
 */
bool seedRunMdnsSlave(int timeoutSeconds = SEED_MDNS_SCAN_TIMEOUT);

/**
 * Check if the seed enable button is pressed on boot.
 * Returns true if the pin is LOW (button pressed, assuming pull-up).
 */
bool seedIsButtonPressedOnBoot();

/**
 * Print the current board mode and status to Serial.
 */
void seedPrintStatus();

#endif // TL4K_SEED_LIBRARY_H
