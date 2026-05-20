/**
 * Tl4k-Main.cpp - TL4K ESP8266 Sensor Board firmware.
 *
 * Boot flow:
 *   1. Load EEPROM config
 *   2. If button pressed on boot → force mDNS slave mode
 *   3. If configured → normal mode (WiFi + mDNS advertiser + MQTT)
 *   4. If NOT configured → mDNS slave mode (scan _tl4k._tcp, fetch config, reboot)
 */

#include "Tl4k-ESP8266-Library.h"
#include "Tl4k-Seed-Config.h"
#include "Tl4k-Seed-Library.h"
#include "Tl4k-WiFi-Library.h"
#include "Tl4k-MQTT-Library.h"

#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

// ============================================================================
// IR Receiver configuration
// ============================================================================
const uint16_t kRecvPin = D3;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 15;
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;
#define LEGACY_TIMING_INFO false

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;

// Track if this board is advertising config via mDNS
static bool isMdnsAdvertiserActive = false;

// ============================================================================
// Setup
// ============================================================================
void setup()
{
  Serial.begin(115200);
  delay(100);  // let Serial stabilize

  Serial.println();
  Serial.println("==========================================");
  Serial.println(" TL4K ESP8266 Sensor Board");
  Serial.println(" BUILD 2026-04-23-B");
  Serial.println("==========================================");

  // Load configuration from EEPROM
  bool wasConfigured = seedConfigLoad();
  seedPrintStatus();

  // Check for forced seed mode (button press on boot)
  bool forceSeedMode = seedIsButtonPressedOnBoot();
  if (forceSeedMode) {
    Serial.println("[BOOT] Button pressed - FORCING mDNS slave mode");
  }

  // ---- SEED SLAVE MODE (unconfigured board) ----
  if (!wasConfigured || forceSeedMode) {
    Serial.println("[BOOT] Board not configured - entering mDNS SLAVE mode...");

    // Knight rider animation while searching
    knightRider(strip.Color(255, 165, 0), 1, 80, 3); // Orange bar

    bool success = seedRunMdnsSlave(30);  // 30 seconds timeout
    if (!success) {
      Serial.println("[BOOT] mDNS slave mode FAILED - will retry on next boot");
      // Blink red to indicate failure
      knightRider(strip.Color(255, 0, 0), 3, 100, 2);
      return;  // stay alive but non-functional
    }
    // If success, ESP.restart() was already called in seedRunMdnsSlave()
    return;  // never reached
  }

  // ---- NORMAL MODE (configured board) ----
  Serial.println("[BOOT] Board is configured - starting normal mode");

  // Initialize sensors
  configuraSchedaEtSensori();

  // Connect to WiFi (uses EEPROM credentials first, falls back to WiFiManager)
  // After WiFi connects, mDNS advertiser is started automatically in WiFi lib
  impostaLaConnessioneWiFi();

  // Connect to MQTT broker (also publishes config to tl4k/config/master)
  connettiAlBrokerMqtt();

  // Check if mDNS advertiser was started (by WiFi lib)
  if (seedConfigIsConfigured() && strlen(tl4kConfig.mqttBroker) > 0) {
    isMdnsAdvertiserActive = true;
  }

  // Success animation
  knightRider(strip.Color(255, 0, 0), 2, 100, 4); // Red Bar

  // Start IR receiver
  #if DECODE_HASH
  irrecv.setUnknownThreshold(kMinUnknownSize);
  #endif
  irrecv.setTolerance(kTolerancePercentage);
  irrecv.enableIRIn();

  stampaUnTestoSuUnaRigaDelMonitorSeriale("-  Configurazione completata  -");
  seedPrintStatus();
}

// ============================================================================
// IR decoding helper
// ============================================================================
String decodeMakeblockIR(decode_results *results) {
  const uint32_t buffer = results->value;
  String output = "";

  for(int i = 0; i < 4; i++) {
    uint8_t byteVal = (buffer >> (8 * i)) & 0xFF;
    if(byteVal >= 0x20 && byteVal <= 0x7E) {
      output += (char)byteVal;
    }
  }

  return output;
}

// ============================================================================
// Loop
// ============================================================================
void loop()
{
  // Keep mDNS advertiser alive
  if (isMdnsAdvertiserActive) {
    seedMdnsAdvertiserLoop();
  }

  Dati dati;
  (void)dati;  // suppress unused variable warning

  // IR receiver
  if (irrecv.decode(&results)) {
    Serial.print("Protocollo: ");
    Serial.println(typeToString(results.decode_type));
    Serial.print("Valore: 0x");

    if (results.decode_type == NEC) {
      String receivedString = "";
      uint32_t data = results.value;

      Serial.print(data, HEX);
      Serial.println("");

      for (int i = 0; i < 4; i++) {
        char c = (data >> (8 * (3 - i))) & 0xFF;
        if (c != 0) receivedString += c;
      }

      Serial.print("Ricevuto: ");
      Serial.println(receivedString);
    }
    else if (results.decode_type == UNKNOWN) {
      String rawString = decodeMakeblockIR(&results);
      Serial.print("Valore hex: 0x");
      Serial.println(results.value, HEX);
      Serial.print("Dati grezzi: ");
      Serial.println(rawString);
    }

    irrecv.resume();
  }

  // MQTT loop (handles reconnect non-blocking)
  mqttBrokerLoop();

  // Feed the Watchdog Timer
  ESP.wdtFeed();

  delay(250);
}
