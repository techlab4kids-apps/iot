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

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 14     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT22     // DHT 22 (AM2302)

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  impostaAgruminoAlRisveglio();
  connettiAlBrokerMqtt();

  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() {

  accendiAgrumino();

  stampaUnTestoSuUnaRigaDelMonitorSeriale("-  Configurazione completata  -");

// Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
  
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
