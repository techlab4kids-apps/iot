#include "Tl4k-ESP8266-Library.h"

#include "Tl4k-WiFi-Library.h"
#include "Tl4k-MQTT-Library.h"

void setup() {
  Serial.begin(115200);

  configuraSchedaEtSensori();

  impostaLaConnessioneWiFi();

  // whiteOverRainbow(75, 2);
  connettiAlBrokerMqtt();
  knightRider(strip.Color(255,  0,  0), 2, 100, 4); // Red Bar
  //faiLampeggiareIlLed();
  stampaUnTestoSuUnaRigaDelMonitorSeriale("-  Configurazione completata  -");
}

void loop() {

  // verificaSeConnessoEtRiconnettiSeNecessario();

  stampaUnTestoSuUnaRigaDelMonitorSeriale("####################################");
  stampaUnaRigaVuotaSulMonitorSeriale();
  
  Dati dati;
  leggiDati(dati);
  
  // stampaDatiSulMonitorSeriale(dati);

  // stampaUnaRigaVuotaSulMonitorSeriale();
  
  // controllaLaConnessioneAlBrokerEtRiconnettiSeNecessario();

  // inviaDatiAlBrokerMqtt(dati);

  //  long distanza = leggiLaDistanza();

   
  //  impostaStrisciaLed(distanza);
   client.loop();
   delay(250);

  //faiLampeggiareIlLed();
}
