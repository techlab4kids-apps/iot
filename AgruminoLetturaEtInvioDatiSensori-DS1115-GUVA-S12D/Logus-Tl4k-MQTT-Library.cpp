#include "Logus-Tl4k-Agrumino-Library.h"
#include "Logus-Tl4k-MQTT-Library.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>


extern Agrumino agrumino;
// extern WiFiClient espClient;

extern PubSubClient client;

void mqttBrokerLoop(){
    client.loop();
}

void reconnect(String clientName) {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("In attesa di connettersi al broker MQTT...");

    // Attempt to connect
    if (client.connect(clientName.c_str(), MQTT_BROKER_USER, "")) {
      Serial.println("Connesso!");

      client.setCallback(on_message);
//
      String topic = MQTT_DEVICE_COMMAND_TOPIC;
      topic.replace("CLIENT_NAME", clientName);
      Serial.println("Pronto a ricevere messaggi su: " + topic);
      client.subscribe(topic.c_str());
    } else {
      Serial.print("Connessione fallita!, rc=");
      Serial.print(client.state());
      Serial.println(" nuovo tentativo tra 5 secondi");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void controllaLaConnessioneAlBrokerEtRiconnettiSeNecessario(){
  checkMqttBrokerConnectionAndReconnectIfRequired();
}

void checkMqttBrokerConnectionAndReconnectIfRequired()
{
  String clientName = getDeviceMqttClientName();
  Serial.print("Connecting to Mqtt Broker with client id: ");
  Serial.println(clientName);

  reconnect(clientName);

  client.loop();
}

void connettiAlBrokerMqtt(){
  connectToMqttBroker();
}

void connectToMqttBroker()
{
  client.setServer(MQTT_BROKER, MQTT_BROKER_PORT);
  //checkMqttBrokerConnectionAndReconnectIfRequired();
}

String getDeviceMqttClientName()
{
  // dmc: Device Mqtt Client ID
  return String(ESP.getChipId());
}

void payloadDeserialize( byte *payload, unsigned int length)
{
  char payloadChars[length + 1];
  strncpy(payloadChars, (char *)payload, length);
  payloadChars[length] = '\0';

  Serial.print("Message: ");
  Serial.println(payloadChars);
  String payloadString = String(payloadChars);

  // Decode JSON request
  StaticJsonDocument<200> doc;

  auto error = deserializeJson(doc, payloadChars);
  if (error)
  {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }
}

void inviaDatiDeiSensoriAgruminoAlBrokerMqtt(DatiDeiSensoriDiAgrumino &agruminoData){
  sendAgruminoDataToMqttBroker(agruminoData);
}

void sendAgruminoDataToMqttBroker(DatiDeiSensoriDiAgrumino &agruminoData)
{
  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"temperatura\":";
  payload += agruminoData.temperature;
  payload += ",";
  payload += "\"umidità del terreno\":";
  payload += agruminoData.soilMoisture;
  payload += ",";
  payload += "\"luminosità\":";
  payload += agruminoData.illuminance;
  payload += ",";
  payload += "\"stato del led\":";
  payload += agruminoData.isLedOn;
  // payload += ",";

  // payload += "\"batteryVoltage\":";
  // payload += agruminoData.batteryVoltage;
  // payload += ",";
  // payload += "\"batteryLevel\":";
  // payload += agruminoData.batteryLevel;
  // payload += ",";
  // payload += "\"isAttachedToUSB\":";
  // payload += agruminoData.isAttachedToUSB;
  // payload += ",";
  // payload += "\"isBatteryCharging\":";
  // payload += agruminoData.isBatteryCharging;
  // payload += ",";

  payload += "}";

  Serial.println("Payload: " + payload);
  Serial.println("Invio dei dati al broker MQTT...");

  String clientName = getDeviceMqttClientName();
  Serial.println("... dal client: " + clientName);

  String topic = MQTT_DEVICE_TELEMETRY_TOPIC; //"v1/devices/" + clientName + "/telemetry";
  topic.replace("CLIENT_NAME", clientName);
  Serial.println("... al topic: " + topic);

  char topicBuffer[200];
  topic.toCharArray(topicBuffer, topic.length() + 1);

  char payloadBuffer[200];
  payload.toCharArray(payloadBuffer, payload.length() + 1);

  bool status = client.publish(topicBuffer, payloadBuffer);
  Serial.println("stato: " + String(status));
}

// The callback for when a PUBLISH message is received from the server.
void on_message(const char *topic, byte *payload, unsigned int length)
{
  Serial.println("On message");

  char payloadChars[length + 1];
  strncpy(payloadChars, (char *)payload, length);
  payloadChars[length] = '\0';

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(payloadChars);
  String payloadString = String(payloadChars);

  Serial.println("methodName: " + payloadString);
  // We expect to recive a JSON in this format
  // name: command name
  // max 5 parameters: {parameter1: parameter1Value, parameter2: parameter2Value, ...}

  const int capacity = JSON_OBJECT_SIZE(1) + 5*JSON_OBJECT_SIZE(1);
  StaticJsonDocument<capacity> commandDoc;
  DeserializationError err = deserializeJson(commandDoc, payloadString);

  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  }

  const char* commandChars = commandDoc["name"];
  String command = String(commandChars);
  Serial.print("Received command: ");
  Serial.println(command.c_str());

  if (command.equals("getLedStatus"))
  {
    DatiDeiSensoriDiAgrumino agruminoData;

    getAgruminoData(agruminoData);

    sendAgruminoDataToMqttBroker(agruminoData);
  }
  else if (command.equals("setLedStatus"))
  {
    bool status = commandDoc["parameters"]["value"];
    Serial.print("New led status: ");
    Serial.println(status);
    setLedStatus(status);
  }
  else if (command.equals("toggleLed"))
  {
    toggleLed();
    DatiDeiSensoriDiAgrumino agruminoData;

    Serial.println("Updating led status data");
    getAgruminoData(agruminoData);

    sendAgruminoDataToMqttBroker(agruminoData);
  }
}
