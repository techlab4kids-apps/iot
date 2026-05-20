/**
 * Tl4k-MQTT-Library.h - MQTT over WebSocket Secure (WSS) library.
 *
 * Uses MQTTPubSubClient + WebSocketsClient (Links2004) for WSS connections.
 * Connects to wss://mqtt.techlab4kids.info/mqtt via WebSocket proxy.
 *
 * Credential priority: EEPROM (seed) > secrets.h > build flags > defaults.
 */

#ifndef _mqttBrokerConfiguration_
#define _mqttBrokerConfiguration_

#include "Tl4k-ESP8266-Library.h"
#include "Tl4k-LedStick-Library.h"
#include "Tl4k-WiFi-Library.h"
#include "Tl4k-Seed-Config.h"
#include <WebSocketsClient.h>
#include <MQTTPubSubClient.h>
#include <ArduinoJson.h>

// External secrets (optional - if secrets.h exists, it overrides defaults)
#ifdef INCLUDE_SECRETS
#include "secrets.h"
#endif

// ============================================================================
// MQTT Broker configuration
// ============================================================================

#ifndef MQTT_BROKER_ADDR
  #define MQTT_BROKER_ADDR "mqtt.techlab4kids.info"
#endif

#ifndef MQTT_BROKER_PORT
  #define MQTT_BROKER_PORT 443
#endif

#ifndef MQTT_BROKER_PATH
  #define MQTT_BROKER_PATH "/mqtt"
#endif

#ifndef MQTT_BROKER_USER
  #define MQTT_BROKER_USER ""
#endif

#ifndef MQTT_BROKER_PASS
  #define MQTT_BROKER_PASS ""
#endif

#ifndef MQTT_SKIP_CERTIFICATE_VALIDATION
// SSL: false = validate CA cert, true = skip validation (testing only)
#define MQTT_SKIP_CERTIFICATE_VALIDATION false
#endif

// MQTT Topics
#define MQTT_DEVICE_TELEMETRY_TOPIC "tl4k/devices/CLIENT_NAME/data"
#define MQTT_DEVICE_COMMAND_TOPIC "tl4k/devices/CLIENT_NAME/command"

// Buffer size for MQTT messages (JSON commands can be large)
#define MQTT_BUFFER_SIZE 512

// ============================================================================
// Public API
// ============================================================================

void mqttBrokerLoop();

String getDeviceMqttClientName();

void inviaDatiAlBrokerMqtt(Dati &data);
void sendDataToMqttBroker(Dati &data);

// The callback for incoming PUBLISH messages (topic, payload, size)
void onMqttMessage(const char* topic, const char* payload, const size_t size);

void reconnect(String clientName);
void controllaLaConnessioneAlBrokerEtRiconnettiSeNecessario();
void checkMqttBrokerConnectionAndReconnectIfRequired();

void connettiAlBrokerMqtt();
void connectToMqttBroker();

#endif // _mqttBrokerConfiguration_
