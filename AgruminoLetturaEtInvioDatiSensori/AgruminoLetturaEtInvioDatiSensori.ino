/*
  AgruminoSample.ino - Sample project for Agrumino board using the Agrumino
  library. Created by giuseppe.broccia@lifely.cc on October 2017.

  This sketch read all the values from the Agrumino Board
  and print them in the serial console every 30 sec.

  @see Agrumino.h for the documentation of the lib
*/

#include "Logus-Tl4k-Agrumino-Library.h"
#include "Logus-Tl4k-WiFi-Library.h"
#include "Logus-Tl4k-MQTT-Library.h"

Agrumino agrumino;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  impostaAgruminoAlRisveglio();
  connettiAlBrokerMqtt();
}

void loop() {

  accendiAgrumino();

  stampaUnTestoSuUnaRigaDelMonitorSeriale("-  Configurazione completata  -");

  verificaSeConnessoEtRiconnettiSeNecessario();

  stampaUnTestoSuUnaRigaDelMonitorSeriale("####################################");
  stampaUnaRigaVuotaSulMonitorSeriale();
  
  DatiDeiSensoriDiAgrumino datiDeiSensoriDiAgrumino;
  leggiDatiDaiSensoriDiAgrumino(datiDeiSensoriDiAgrumino);
  
  stampaDatiDaAgruminoSulMonitorSeriale(datiDeiSensoriDiAgrumino);

  stampaUnaRigaVuotaSulMonitorSeriale();
  
  controllaLaConnessioneAlBrokerEtRiconnettiSeNecessario();

  inviaDatiDeiSensoriAgruminoAlBrokerMqtt(datiDeiSensoriDiAgrumino);

  faiLampeggiareIlLed();

  addormentaProfondamenteAgrumino();
}
