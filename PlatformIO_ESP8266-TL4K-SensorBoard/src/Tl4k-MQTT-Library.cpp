/**
 * Tl4k-MQTT-Library.cpp - MQTT over WebSocket Secure (WSS) implementation.
 *
 * Uses MQTTPubSubClient + WebSocketsClient (Links2004) for WSS.
 * Connects to wss://mqtt.techlab4kids.info/mqtt via WebSocket proxy.
 *
 * Architecture:
 *   WiFi → WebSocketsClient (WSS/TLS) → MQTTPubSubClient (MQTT 3.1.1 over WS)
 */

#include "Tl4k-MQTT-Library.h"
#include "Tl4k-MQTT-Certificate.h"
#include <WebSocketsClient.h>
#include <MQTTPubSubClient.h>
#include <ArduinoJson.h>

// WebSocket client for WSS
WebSocketsClient wsClient;

// MQTT client wrapping the WebSocket (with configurable buffer size)
MQTTPubSub::PubSubClient<MQTT_BUFFER_SIZE> mqttClient;

// Track connection state and timing
static bool mqttInitialized = false;
static unsigned long lastMqttAttemptTime = 0;
static const unsigned long MQTT_RECONNECT_INTERVAL = 5000;
static unsigned long lastMqttLogTime = 0;
static const unsigned long MQTT_LOG_INTERVAL = 15000;

// Track broker config
static String mqttBrokerHost = MQTT_BROKER_ADDR;
static uint16_t mqttBrokerPort = MQTT_BROKER_PORT;
static String mqttBrokerPath = MQTT_BROKER_PATH;
static String mqttUser = MQTT_BROKER_USER;
static String mqttPassword = MQTT_BROKER_PASS;

// Track subscribed topic for callback
static String lastSubscribedTopic;

// ============================================================================
// Initialize MQTT client over WSS
// ============================================================================

static void wsEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WS] DISCONNECTED");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WS] CONNECTED to %s\n", payload);
      break;
    case WStype_TEXT:
      Serial.printf("[WS] TEXT (%d bytes)\n", length);
      break;
    case WStype_BIN:
      Serial.printf("[WS] BIN (%d bytes)\n", length);
      break;
    case WStype_ERROR:
      Serial.println("[WS] ERROR");
      break;
    default:
      Serial.printf("[WS] event type=%d\n", type);
      break;
  }
}

static void initMqttClient() {
  if (mqttInitialized) return;

  Serial.printf("[MQTT] Configuring WSS: %s:%d%s\n",
    mqttBrokerHost.c_str(), mqttBrokerPort, mqttBrokerPath.c_str());

  wsClient.onEvent(wsEvent);

#if MQTT_SKIP_CERTIFICATE_VALIDATION
  Serial.println("[MQTT] SSL: insecure mode (no certificate validation)");
  wsClient.beginSSL(mqttBrokerHost.c_str(), mqttBrokerPort, mqttBrokerPath.c_str());
#else
  Serial.println("[MQTT] SSL: validating server certificate against CA chain");
  wsClient.beginSslWithCA(
    mqttBrokerHost.c_str(),
    mqttBrokerPort,
    mqttBrokerPath.c_str(),
    MQTT_CA_CERT
  );
#endif

  wsClient.setReconnectInterval(2000);

  mqttClient.begin(wsClient);

  mqttInitialized = true;
}

// ============================================================================
// Publish config to MQTT topic for slave discovery
// ============================================================================

static void publishConfigToMqtt(const String& deviceId) {
  JsonDocument doc;
  doc["broker_url"]   = mqttBrokerHost;
  doc["broker_port"]   = mqttBrokerPort;
  doc["broker_path"]   = mqttBrokerPath;
  doc["mqtt_user"]     = mqttUser;
  doc["mqtt_pass"]     = mqttPassword;
  doc["config_version"] = EEPROM_VERSION;
  doc["master_id"]     = deviceId;

  String json;
  serializeJson(doc, json);

  String topic = "tl4k/config/master";
  Serial.printf("[MQTT] Publishing config to %s\n", topic.c_str());
  mqttClient.publish(topic.c_str(), json.c_str());
}

// ============================================================================
// Internal connect helper
// ============================================================================

static bool doMqttConnect(const String& clientId) {
  Serial.printf("[MQTT] Connecting as: %s\n", clientId.c_str());

  bool ok;
  if (mqttUser.length() > 0 && mqttPassword.length() > 0) {
    ok = mqttClient.connect(clientId.c_str(), mqttUser.c_str(), mqttPassword.c_str());
  } else if (mqttUser.length() > 0) {
    ok = mqttClient.connect(clientId.c_str(), mqttUser.c_str(), "");
  } else {
    ok = mqttClient.connect(clientId.c_str());
  }

  if (ok) {
    Serial.println("[MQTT] Connected via WSS!");

    // Subscribe to command topic with callback
    String topic = MQTT_DEVICE_COMMAND_TOPIC;
    topic.replace("CLIENT_NAME", clientId);
    lastSubscribedTopic = topic;
    Serial.printf("[MQTT] Subscribing to: %s\n", topic.c_str());

    // Subscribe with lambda - uses static topic variable
    mqttClient.subscribe(topic.c_str(), [](const char* payload, const size_t size) {
      onMqttMessage(lastSubscribedTopic.c_str(), payload, size);
    });

    // Publish config to MQTT for other devices
    publishConfigToMqtt(clientId);

    return true;
  } else {
    Serial.printf("[MQTT] Connection failed\n");
    return false;
  }
}

// ============================================================================
// Loop
// ============================================================================

void mqttBrokerLoop() {
  wsClient.loop();
  mqttClient.update();

  if (!mqttClient.isConnected()) {
    if (millis() - lastMqttLogTime > MQTT_LOG_INTERVAL) {
      lastMqttLogTime = millis();
      Serial.printf("[MQTT] Not connected (heap=%u), retrying...\n", ESP.getFreeHeap());
    }
    String clientName = getDeviceMqttClientName();
    reconnect(clientName);
  }
}

// ============================================================================
// Connect / Reconnect
// ============================================================================

void reconnect(String clientName) {
  // Non-blocking: only attempt if interval has elapsed
  if (millis() - lastMqttAttemptTime < MQTT_RECONNECT_INTERVAL) {
    return;
  }

  if (!mqttClient.isConnected()) {
    lastMqttAttemptTime = millis();

    if (!mqttInitialized) {
      initMqttClient();
    }

    doMqttConnect(clientName);
  }
}

void controllaLaConnessioneAlBrokerEtRiconnettiSeNecessario() {
  checkMqttBrokerConnectionAndReconnectIfRequired();
}

void checkMqttBrokerConnectionAndReconnectIfRequired() {
  String clientName = getDeviceMqttClientName();
  Serial.printf("[MQTT] Checking connection: %s\n", clientName.c_str());
  reconnect(clientName);
}

// ============================================================================
// Initial Connection
// ============================================================================

void connettiAlBrokerMqtt() {
  connectToMqttBroker();
}

void connectToMqttBroker() {
  // Load credentials: EEPROM > macros
  if (seedConfigIsConfigured() && strlen(tl4kConfig.mqttBroker) > 0) {
    mqttBrokerHost   = String(tl4kConfig.mqttBroker);
    mqttBrokerPort   = (uint16_t)tl4kConfig.mqttPort;
    mqttBrokerPath   = MQTT_BROKER_PATH;
    mqttUser         = String(tl4kConfig.mqttUser);
    mqttPassword     = String(tl4kConfig.mqttPass);
    Serial.printf("[MQTT] Using EEPROM credentials: %s:%d%s\n",
                  mqttBrokerHost.c_str(), mqttBrokerPort, mqttBrokerPath.c_str());
  } else {
    mqttBrokerHost   = MQTT_BROKER_ADDR;
    mqttBrokerPort   = MQTT_BROKER_PORT;
    mqttBrokerPath   = MQTT_BROKER_PATH;
    mqttUser         = MQTT_BROKER_USER;
    mqttPassword     = MQTT_BROKER_PASS;
    Serial.printf("[MQTT] Using fallback credentials: %s:%d%s\n",
                  mqttBrokerHost.c_str(), mqttBrokerPort, mqttBrokerPath.c_str());
  }

  initMqttClient();

  // Connect
  String clientName = getDeviceMqttClientName();
  bool ok = doMqttConnect(clientName);

  if (!ok) {
    Serial.println("[MQTT] Initial connection pending (will retry in background)");
  }
}

// ============================================================================
// Utilities
// ============================================================================

String getDeviceMqttClientName() {
  return String(ESP.getChipId());
}

// ============================================================================
// Publish data to MQTT broker
// ============================================================================

void inviaDatiAlBrokerMqtt(Dati &data) {
  sendDataToMqttBroker(data);
}

void sendDataToMqttBroker(Dati &data) {
  if (!mqttClient.isConnected()) {
    Serial.println("[MQTT] ERROR: Not connected, cannot publish");
    return;
  }

  // Prepare JSON payload
  String payload = "{\"distanza\":";
  payload += data.distanza;
  payload += "}";

  String clientName = getDeviceMqttClientName();
  String topic = MQTT_DEVICE_TELEMETRY_TOPIC;
  topic.replace("CLIENT_NAME", clientName);

  Serial.printf("[MQTT] Publishing to %s: %s\n", topic.c_str(), payload.c_str());

  mqttClient.publish(topic.c_str(), payload.c_str());
}

// ============================================================================
// Message callback (incoming commands)
// ============================================================================

void onMqttMessage(const char* topic, const char* payload, const size_t size) {
  Serial.println("[MQTT] On message");
  Serial.printf("[MQTT] Topic: %s\n", topic);

  // Make null-terminated copy of payload
  char payloadChars[size + 1];
  memcpy(payloadChars, payload, size);
  payloadChars[size] = '\0';

  Serial.printf("[MQTT] Message: %s\n", payloadChars);

  // Parse JSON command
  JsonDocument commandDoc;
  DeserializationError err = deserializeJson(commandDoc, payloadChars);

  if (err) {
    Serial.printf("[MQTT] deserializeJson() failed: %s\n", err.c_str());
    return;
  }

  // Validate command field
  if (!commandDoc["comando"].is<const char*>()) {
    Serial.println("[MQTT] ERROR: Missing 'comando' field");
    return;
  }

  String command = String(commandDoc["comando"].as<const char*>());
  Serial.printf("[MQTT] Command: %s\n", command.c_str());

  // ========================================================================
  // Command handlers
  // ========================================================================

  if (command == "show") {
    Serial.println("[MQTT] Command: show");
    strip.show();
  }
  else if (command == "color") {
    Serial.println("[MQTT] Command: color");
    int red = commandDoc["parametri"]["red"];
    int green = commandDoc["parametri"]["green"];
    int blue = commandDoc["parametri"]["blue"];
    Serial.printf("[MQTT] Color: R%d G%d B%d\n", red, green, blue);
  }
  else if (command == "setPixelColor") {
    Serial.println("[MQTT] Command: setPixelColor");
    int idx = commandDoc["parametri"]["indice"];
    int red = commandDoc["parametri"]["red"];
    int green = commandDoc["parametri"]["green"];
    int blue = commandDoc["parametri"]["blue"];
    Serial.printf("[MQTT] Index: %d Color: R%d G%d B%d\n", idx, red, green, blue);
    strip.setPixelColor(idx, red, green, blue);
  }
  else if (command == "setBrightness") {
    Serial.println("[MQTT] Command: setBrightness");
    int brightness = commandDoc["parametri"]["luminosita"];
    Serial.printf("[MQTT] Brightness: %d\n", brightness);
    strip.setBrightness(brightness);
  }
  else if (command == "clear") {
    Serial.println("[MQTT] Command: clear");
    strip.clear();
  }
  else if (command == "resetConfig") {
    Serial.println("[MQTT] Command: resetConfig");
    seedConfigReset();
    Serial.println("[MQTT] Rebooting...");
    delay(1000);
    ESP.restart();
  }
  else if (command == "colorWipe") {
    Serial.println("[MQTT] Command: colorWipe");
    int red = commandDoc["parametri"]["red"];
    int green = commandDoc["parametri"]["green"];
    int blue = commandDoc["parametri"]["blue"];
    int wait = commandDoc["parametri"]["attesa"];
    colorWipe(strip.Color(red, green, blue), wait);
  }
  else if (command == "knightRider") {
    Serial.println("[MQTT] Command: knightRider");
    int red = commandDoc["parametri"]["red"];
    int green = commandDoc["parametri"]["green"];
    int blue = commandDoc["parametri"]["blue"];
    uint32_t color = strip.Color(red, green, blue);
    int reps = commandDoc["parametri"]["ripetizioni"];
    int wait = commandDoc["parametri"]["attesa"];
    int len = commandDoc["parametri"]["lunghezza"];
    knightRider(color, reps, wait, len);
  }
  else {
    Serial.printf("[MQTT] Unknown command: %s\n", command.c_str());
  }
}
