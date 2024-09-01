/*
  Progetto di esempio basato su AgruminoSample di giuseppe.broccia@lifely.cc.

  Modifiche e italianizzazione di Giancarlo Orrù (giancarlo.orru@techlab4kids.it) per Logus

  Il progetto consente di visualizzare sul monitor seriale dell'IDE Arduino i dati letti
  dai sensori agrumino
*/

#include "Logus-Tl4k-Agrumino-Library.h"

Agrumino agrumino;

void setup() {
  impostaAgruminoAlRisveglio();
}

void loop() {
  stampaUnTestoSuUnaRigaDelMonitorSeriale("####################################");
  stampaUnaRigaVuotaSulMonitorSeriale();

  accendiAgrumino();

  stampaUnaRigaVuotaSulMonitorSeriale();
  
  DatiDeiSensoriDiAgrumino datiDeiSensoriDiAgrumino;
  leggiDatiDaiSensoriDiAgrumino(datiDeiSensoriDiAgrumino);
  
  stampaDatiDaAgruminoSulMonitorSeriale(datiDeiSensoriDiAgrumino);

  stampaUnaRigaVuotaSulMonitorSeriale();

  faiLampeggiareIlLed();

  addormentaProfondamenteAgrumino();
}
