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

// BMP085 begin
#include "BMP085.h"
#include <Wire.h>
float temperature;
float pressure;
float atm;
float altitude;
BMP085 myBarometer;
// BMP085 end

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  impostaAgruminoAlRisveglio();
  connettiAlBrokerMqtt();

// BMP085 begin
 myBarometer.init();
// BMP085 end
 
}

void loop() {

  accendiAgrumino();

  stampaUnTestoSuUnaRigaDelMonitorSeriale("-  Configurazione completata  -");

// BMP085 begin
    temperature = myBarometer.bmp085GetTemperature(
                      myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
    pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());//Get the temperature

    /*
        To specify a more accurate altitude, enter the correct mean sea level
        pressure level.  For example, if the current pressure level is 1019.00 hPa
        enter 101900 since we include two decimal places in the integer value。
    */
    altitude = myBarometer.calcAltitude(101900);

    atm = pressure / 101325;

    Serial.print("Temperature: ");
    Serial.print(temperature, 2); //display 2 decimal places
    Serial.println(" Celsius");

    Serial.print("Pressure: ");
    Serial.print(pressure, 0); //whole number only.
    Serial.println(" Pa");

    Serial.print("Ralated Atmosphere: ");
    Serial.println(atm, 4); //display 4 decimal places

    Serial.print("Altitude: ");
    Serial.print(altitude, 2); //display 2 decimal places
    Serial.println(" m");
// BMP085 end
  
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
