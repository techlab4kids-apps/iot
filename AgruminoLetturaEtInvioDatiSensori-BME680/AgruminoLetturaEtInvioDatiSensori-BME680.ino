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

// Bme680  begin
#include "seeed_bme680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10


#define IIC_ADDR  uint8_t(0x76)

Seeed_BME680 bme680(IIC_ADDR); /* IIC PROTOCOL */

// Bme680  end

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  impostaAgruminoAlRisveglio();
  connettiAlBrokerMqtt();

// Bme680 begin
  while (!bme680.init()) {
        Serial.println("bme680 init failed ! can't find device!");
        delay(10000);
    }
// Bme680 end
 
}

void loop() {

  accendiAgrumino();

  stampaUnTestoSuUnaRigaDelMonitorSeriale("-  Configurazione completata  -");


// Bme680 begin
    Serial.print("wait 3 secs");
    delay(3000);

   if (bme680.read_sensor_data()) {
        Serial.println("Failed to perform reading :(");
        return;
    }
    Serial.print("temperature ===>> ");
    Serial.print(bme680.sensor_result_value.temperature);
    Serial.println(" C");

    Serial.print("pressure ===>> ");
    Serial.print(bme680.sensor_result_value.pressure / 1000.0);
    Serial.println(" KPa");

    Serial.print("humidity ===>> ");
    Serial.print(bme680.sensor_result_value.humidity);
    Serial.println(" %");

    Serial.print("gas ===>> ");
    Serial.print(bme680.sensor_result_value.gas / 1000.0);
    Serial.println(" Kohms");
// Bme680 end
  
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
