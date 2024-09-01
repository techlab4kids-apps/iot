/*
  Progetto di esempio basato su AgruminoSample di giuseppe.broccia@lifely.cc.

  Modifiche e italianizzazione di Giancarlo Orrù (giancarlo.orru@techlab4kids.it) per Logus

  Il progetto consente di visualizzare sul monitor seriale dell'IDE Arduino i dati letti
  dai sensori agrumino
*/

#include "Logus-Tl4k-Agrumino-Library.h"

#include <LedControl.h>

int DIN = GPIO_2; // D0

int CS =  GPIO_1 ;  // D1

int CLK = 14 ;  // D2

LedControl lc=LedControl(DIN,CLK,CS,0);

Agrumino agrumino;

void setup() {
  impostaAgruminoAlRisveglio();

  agrumino.setGPIOMode(GPIO_1, GPIO_OUTPUT);
  agrumino.setGPIOMode(GPIO_2, GPIO_OUTPUT);

  lc.shutdown(0,false);     //The MAX72XX is in power-saving mode on startup

  lc.setIntensity(0,15);     // Set the brightness to maximum value

  lc.clearDisplay(0);        // and clear the display
  
}

void loop() {
  stampaUnTestoSuUnaRigaDelMonitorSeriale("####################################");
  stampaUnaRigaVuotaSulMonitorSeriale();

  accendiAgrumino();

  byte a[8] = {0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xFF,0xFF};  // L
    byte b[8] = {0xFF,0xFF,0x18,0x18,0x18,0x18,0xFF,0xFF};     // I
    byte c[8] = {0x7F,0xFF,0xC0,0xDF,0xDF,0xC3,0x7F,0x3F};   // G
    byte d[8] = {0xC3,0xC3,0xC3,0xFF,0xFF,0xC3,0xC3,0xC3}; // H
    byte e[8] = {0xFF,0xFF,0x18,0x18,0x18,0x18,0x18,0x18};    // T
    
    byte f[8] = {0xC3,0xC3,0xC3,0xFF,0xFF,0xC3,0xC3,0xC3};  // H
    byte g[8] = {0x3C,0x7E,0xC3,0xC3,0xC3,0xC3,0x7E,0x3C}; // O
    byte h[8] = {0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF};  // U
    byte i[8] = {0x7F,0xFE,0xC0,0xFE,0x7F,0x03,0x7F,0xFE};     // S
    byte j[8] = {0xFF,0xFF,0xC0,0xF8,0xF8,0xC0,0xFF,0xFF};     // E
    printByte(a);
    delay(1000);
    printByte(b);
    delay(1000);

  stampaUnaRigaVuotaSulMonitorSeriale();
  
  DatiDeiSensoriDiAgrumino datiDeiSensoriDiAgrumino;
  leggiDatiDaiSensoriDiAgrumino(datiDeiSensoriDiAgrumino);
  
  stampaDatiDaAgruminoSulMonitorSeriale(datiDeiSensoriDiAgrumino);

  stampaUnaRigaVuotaSulMonitorSeriale();

  faiLampeggiareIlLed();

  addormentaProfondamenteAgrumino();
}

void printByte(byte character []){
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
