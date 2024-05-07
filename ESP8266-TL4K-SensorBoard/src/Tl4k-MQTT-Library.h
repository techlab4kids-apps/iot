#ifndef _mqttBrokerConfiguration_
#define _mqttBrokerConfiguration_

#include "Tl4k-ESP8266-Library.h"
#include "Tl4k-LedStick-Library.h"
#include "Tl4k-WiFi-Library.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Mqtt parameters
//#define MQTT_BROKER "mqtt.flespi.io"
//#define MQTT_BROKER_PORT 1883
//#define MQTT_BROKER_USER "FlespiToken ZZUuFrUpQdGIC5uV34q0esnHVEqzQnyGaiYGzITdJpKcl2VeVb2XudvdHJAmUYQV"
//#define MQTT_BROKER_PASSWORD ""


//#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_BROKER "192.168.0.112"
#define MQTT_BROKER_PORT 1883
#define MQTT_BROKER_USER ""
#define MQTT_BROKER_PASSWORD ""

#define MQTT_DEVICE_TELEMETRY_TOPIC "tl4k/devices/CLIENT_NAME/data";
#define MQTT_DEVICE_COMMAND_TOPIC "tl4k/devices/CLIENT_NAME/command";

void mqttBrokerLoop();

String getDeviceMqttClientName();

void payloadDeserialize( byte *payload, unsigned int length);

void inviaDatiAlBrokerMqtt(Dati &data);
void sendDataToMqttBroker(Dati &data);

// The callback for when a PUBLISH message is received from the server.
void on_message(const char *topic, byte *payload, unsigned int length);

void reconnect(String clientName);
void controllaLaConnessioneAlBrokerEtRiconnettiSeNecessario();
void checkMqttBrokerConnectionAndReconnectIfRequired();

void connettiAlBrokerMqtt();
void connectToMqttBroker();

#endif //_mqttBrokerConfiguration_
