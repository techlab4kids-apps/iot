
#include "Tl4k-WiFi-Library.h" 

// the Wifi radio's status
int status = WL_IDLE_STATUS;

WiFiManager wifiManager;

void impostaLaConnessioneWiFi(){
   initWiFi();
}

void initWiFi()
{
  
  // WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  Serial.println("Connecting to AP ...");
  wifiManager.autoConnect("WIFI_AP_NAME", "WIFI_PASSWORD");  
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    delay(500);
//    Serial.print(".");
//  }
  Serial.println("Connected to AP");
  //Serial.print("\nLocal ip address is ");
  //Serial.println(WiFi.localIP());
  Serial.println();

}

void reconnectWifi()
{
  // Loop until we're reconnected
  status = WiFi.status();
  Serial.println("Trying to Re-connect to AP");
  if (status != WL_CONNECTED)
  {
    WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");

    Serial.print("\nLocal ip address is ");
    Serial.println(WiFi.localIP());
    Serial.println();

    //testHttpConnection();
  }
}

void testHttpConnection(){
  const char* url = "https://api.ipify.org";

  RequestOptions options;
  options.method = "GET";

  Response response = fetch(url, options);

  if (response.ok)
  {
    String data = response.text();
    Serial.println("Response OK: \n" + data);
  }
  else{
    String statusText = response.statusText;
    Serial.println("Error:\n" + statusText);
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
