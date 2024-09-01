#ifndef _mqttBrokerConfiguration_
#define _mqttBrokerConfiguration_

#include "Logus-Tl4k-Agrumino-Library.h"
#include "Logus-Tl4k-WiFi-Library.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Mqtt parameters
#define MQTT_BROKER "mqtt.flespi.io"
#define MQTT_BROKER_PORT 1883
#define MQTT_BROKER_USER "FlespiToken sqiHeAwQBrlgWmeZtpeRsGENY1pO11A5f7djC5SUmkfpcrVyuf2kgnYW6JacdjVV"
#define MQTT_BROKER_PASSWORD ""


//#define MQTT_BROKER "test.mosquitto.org"
//#define MQTT_BROKER "192.168.0.111"
//#define MQTT_BROKER_PORT 1883
//#define MQTT_BROKER_USER ""
//#define MQTT_BROKER_PASSWORD ""

#define MQTT_DEVICE_TELEMETRY_TOPIC "tl4k/devices/CLIENT_NAME/data";
#define MQTT_DEVICE_COMMAND_TOPIC "tl4k/devices/CLIENT_NAME/command";

extern Agrumino agrumino;
// extern WiFiClient espClient;

void mqttBrokerLoop();

String getDeviceMqttClientName();

void payloadDeserialize( byte *payload, unsigned int length);

void inviaDatiDeiSensoriAgruminoAlBrokerMqtt(DatiDeiSensoriDiAgrumino &agruminoData);
void sendAgruminoDataToMqttBroker(DatiDeiSensoriDiAgrumino &agruminoData);

// The callback for when a PUBLISH message is received from the server.
void on_message(const char *topic, byte *payload, unsigned int length);

void reconnect(String clientName);
void controllaLaConnessioneAlBrokerEtRiconnettiSeNecessario();
void checkMqttBrokerConnectionAndReconnectIfRequired();

void connettiAlBrokerMqtt();
void connectToMqttBroker();

#endif //_mqttBrokerConfiguration_
