
#include "Tl4k-WiFi-Library.h" 

// the Wifi radio's status
int status = WL_IDLE_STATUS;

WiFiManager wifiManager;

void impostaLaConnessioneWiFi(){
   initWiFi();
}

void initWiFi()
{

  WiFiManagerParameter custom_mqtt_server_name("mqtt_server_name", "mqtt server name", "localhost", 40);
  wifiManager.addParameter(&custom_mqtt_server_name);
  WiFiManagerParameter custom_mqtt_server_port("mqtt_server_port", "mqtt server port", "1883", 6);
  wifiManager.addParameter(&custom_mqtt_server_port);
  
  // WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  Serial.println("Connessione all'Access Point...");
  // wifiManager.autoConnect(WIFI_AP_NAME, WIFI_PASSWORD);  
  
  if (!wifiManager.autoConnect(WIFI_AP_NAME, WIFI_PASSWORD)) {
    Serial.println("Tentativo di connessione fallito");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    Serial.println("Reset...");
    ESP.reset();
    delay(5000);
    Serial.println("Post reset");
  }

//  while (WiFi.status() != WL_CONNECTED)
//  {
//    delay(500);
//    Serial.print(".");
//  }
  // Serial.println("Connected to AP");
  //Serial.print("\nLocal ip address is ");
  //Serial.println(WiFi.localIP());
  Serial.println();

}

void reconnectWifi()
{
  // Loop until we're reconnected
  status = WiFi.status();
  Serial.println("Tentativo di riconnessione all'Access Point in corso");
  if (status != WL_CONNECTED)
  {
    WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connessione completata!");

    Serial.print("\nL'indirizzo IP locale è ");
    Serial.println(WiFi.localIP());
    Serial.println();

    //testHttpConnection();
  }
}

void verificaSeConnessoEtRiconnettiSeNecessario(){
  checkWifiConnectionAndRecconnectIfNecessary();
}

void checkWifiConnectionAndRecconnectIfNecessary()
{
  // Reconnect to WiFi, if needed
  bool isWifiConnected = WiFi.status() != WL_CONNECTED;
  if (isWifiConnected)
  {
    Serial.println("Connecting to: WIFI");
    reconnectWifi();
    return;
  }
}
