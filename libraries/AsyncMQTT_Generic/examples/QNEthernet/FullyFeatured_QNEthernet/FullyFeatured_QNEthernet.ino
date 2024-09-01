/****************************************************************************************************************************
  FullyFeatured_QNEthernet.ino

  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, Teensy41_QNEthernet_PubF7, etc. with current AsyncTCP support

  Based on and modified from :

  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)

  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#include "defines.h"

// Check connection every 1s
#define MQTT_CHECK_INTERVAL_MS     1000

#include <AsyncMqtt_Generic.h>

#include <Ticker.h>                   // https://github.com/sstaub/Ticker

//#define MQTT_HOST         IPAddress(192, 168, 2, 110)
#define MQTT_HOST         "broker.emqx.io"        // Broker address
#define MQTT_PORT         1883

const char *PubTopic  = "async-mqtt/Teensy41_QNEthernet_Pub";               // Topic to publish

AsyncMqttClient mqttClient;

void connectToMqtt();
void connectToMqttCheck();

// Repeat forever, millis() resolution
Ticker connectToMqttTicker(connectToMqttCheck, MQTT_CHECK_INTERVAL_MS, 0, MILLIS);

bool connectedEthernet  = false;
bool connectedMQTT      = false;

void connectToMqttCheck()
{
  if (Ethernet.linkStatus() == LinkON)
  {
    //Ethernet.maintain();

    if (!connectedMQTT)
    {
      mqttClient.connect();
    }

    if (!connectedEthernet)
    {
      Serial.println("Ethernet reconnected.");
      connectedEthernet = true;
    }
  }
  else
  {
    if (connectedEthernet)
    {
      Serial.println("Ethernet disconnected");
      connectedEthernet = false;
    }
  }
}

void connectToMqtt()
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void printSeparationLine()
{
  Serial.println("************************************************");
}

void onMqttConnect(bool sessionPresent)
{
  Serial.print("Connected to MQTT broker: ");
  Serial.print(MQTT_HOST);
  Serial.print(", port: ");
  Serial.println(MQTT_PORT);
  Serial.print("PubTopic: ");
  Serial.println(PubTopic);

  connectedMQTT = true;

  printSeparationLine();
  Serial.print("Session present: ");
  Serial.println(sessionPresent);

  uint16_t packetIdSub = mqttClient.subscribe(PubTopic, 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);

  mqttClient.publish(PubTopic, 0, true, "Teensy41_QNEthernet_Pub Test1");
  Serial.println("Publishing at QoS 0");

  uint16_t packetIdPub1 = mqttClient.publish(PubTopic, 1, true, "Teensy41_QNEthernet_Pub Test2");
  Serial.print("Publishing at QoS 1, packetId: ");
  Serial.println(packetIdPub1);

  uint16_t packetIdPub2 = mqttClient.publish(PubTopic, 2, true, "Teensy41_QNEthernet_Pub Test3");
  Serial.print("Publishing at QoS 2, packetId: ");
  Serial.println(packetIdPub2);

  printSeparationLine();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  (void) reason;

  connectedMQTT = false;

  Serial.println("Disconnected from MQTT.");
}

void onMqttSubscribe(const uint16_t& packetId, const uint8_t& qos)
{
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(const uint16_t& packetId)
{
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, const AsyncMqttClientMessageProperties& properties,
                   const size_t& len, const size_t& index, const size_t& total)
{
  char message[len + 1];

  memcpy(message, payload, len);
  message[len] = 0;

  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  message: ");
  Serial.println(message);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
}

void onMqttPublish(const uint16_t& packetId)
{
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(500);

  Serial.print("\nStarting FullyFeatured_QNEthernet on ");
  Serial.println(BOARD_NAME);
  Serial.println(ASYNC_MQTT_GENERIC_VERSION);

#if USING_DHCP
  // Start the Ethernet connection, using DHCP
  Serial.print("Initialize Ethernet using DHCP => ");
  Ethernet.begin();
#else
  // Start the Ethernet connection, using static IP
  Serial.print("Initialize Ethernet using static IP => ");
  Ethernet.begin(myIP, myNetmask, myGW);
  Ethernet.setDNSServerIP(mydnsServer);
#endif

  if (!Ethernet.waitForLocalIP(5000))
  {
    Serial.println(F("Failed to configure Ethernet"));

    if (!Ethernet.linkStatus())
    {
      Serial.println(F("Ethernet cable is not connected."));
    }

    // Stay here forever
    while (true)
    {
      delay(1);
    }
  }
  else
  {
    Serial.print(F("Connected! IP address:"));
    Serial.println(Ethernet.localIP());
  }

#if USING_DHCP
  delay(1000);
#else
  delay(2000);
#endif

  connectedEthernet = true;

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToMqttTicker.start(); //start the ticker.

  connectToMqtt();
}

void loop()
{
  connectToMqttTicker.update(); //update the ticker.
}
