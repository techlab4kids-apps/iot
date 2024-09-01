/*
  Progetto di esempio basato su AgruminoSample di giuseppe.broccia@lifely.cc.

  Modifiche e italianizzazione di Giancarlo Orrù (giancarlo.orru@techlab4kids.it) per Logus

  Il progetto consente di visualizzare sul monitor seriale dell'IDE Arduino i dati letti
  dai sensori agrumino
*/

#include "Logus-Tl4k-Agrumino-Library.h"

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 1
//#define CLK_PIN   13  // or SCK
#define DATA_PIN  11  // or MOSI
#define CS_PIN    10  // or SS

MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

Agrumino agrumino;

void setup() {
  impostaAgruminoAlRisveglio();

  agrumino.setGPIOMode(GPIO_1, GPIO_OUTPUT);
  agrumino.setGPIOMode(GPIO_2, GPIO_OUTPUT);

  Display.begin();
  Display.setIntensity(0);
  Display.displayClear();
  
}

void loop() {
  stampaUnTestoSuUnaRigaDelMonitorSeriale("####################################");
  stampaUnaRigaVuotaSulMonitorSeriale();

  accendiAgrumino();

  Display.setTextAlignment(PA_LEFT);
  Display.print("ESP");
  delay(2000);
  
  Display.setTextAlignment(PA_CENTER);
  Display.print("ESP");
  delay(2000);

  Display.setTextAlignment(PA_RIGHT);
  Display.print("ESP");
  delay(2000);

  Display.setTextAlignment(PA_CENTER);
  Display.setInvert(true);
  Display.print("ESP");
  delay(2000);

  Display.setInvert(false);
  delay(2000);

  stampaUnaRigaVuotaSulMonitorSeriale();
  
  DatiDeiSensoriDiAgrumino datiDeiSensoriDiAgrumino;
  leggiDatiDaiSensoriDiAgrumino(datiDeiSensoriDiAgrumino);
  
  stampaDatiDaAgruminoSulMonitorSeriale(datiDeiSensoriDiAgrumino);

  stampaUnaRigaVuotaSulMonitorSeriale();

  faiLampeggiareIlLed();

  addormentaProfondamenteAgrumino();
}
