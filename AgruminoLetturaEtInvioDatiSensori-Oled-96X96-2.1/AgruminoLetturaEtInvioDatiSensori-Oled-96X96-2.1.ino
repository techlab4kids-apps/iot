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

// Oled 96x96 begin
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

U8G2_SH1107_SEEED_128X128_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
// Oled 96x96 end

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  impostaAgruminoAlRisveglio();
  connettiAlBrokerMqtt();

 // Oled 96x96 begin
 u8g2.begin();
 // Oled 96x96 end
}

void loop() {

  accendiAgrumino();

  stampaUnTestoSuUnaRigaDelMonitorSeriale("-  Configurazione completata  -");

// Oled 96x96 begin
u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0,24,"Hello World!");
  } while ( u8g2.nextPage() );
// Oled 96x96 end
  
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
