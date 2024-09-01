#ifndef LOGUS_TL4K_AGRUMINO_LIBRARY_H
#define LOGUS_TL4K_AGRUMINO_LIBRARY_H

#include <Agrumino.h>

#define CORREZIONE_DELLA_TEMPERATURA 9

#define DURATA_DEL_SONNO_IN_SEC 5


typedef struct AgruminoData_t
{
  float temperature;
  unsigned int soilMoisture;
  float illuminance;
  float batteryVoltage;
  unsigned int batteryLevel;
  boolean isAttachedToUSB;
  boolean isBatteryCharging;
  boolean isLedOn;
  String ledColor;
} DatiDeiSensoriDiAgrumino;

extern Agrumino agrumino;

void impostaAgruminoAlRisveglio();

void stampaUnaRigaVuotaSulMonitorSeriale();
void stampaUnTestoSuUnaRigaDelMonitorSeriale(const char * testo);

void accendiAgrumino();

void addormentaProfondamenteAgrumino();
void addormentaAgrumino();

void leggiDatiDaiSensoriDiAgrumino(DatiDeiSensoriDiAgrumino &agruminoData);
void stampaDatiDaAgruminoSulMonitorSeriale(DatiDeiSensoriDiAgrumino &agruminoData);

void getAgruminoData(DatiDeiSensoriDiAgrumino &agruminoData);
void printAgruminoData(DatiDeiSensoriDiAgrumino &agruminoData);

float agruminoGetTemperature();

void faiLampeggiareIlLed();
void blinkLed();
void delaySec(int sec);
void deepSleepSec(int sec);

String isLedOn(DatiDeiSensoriDiAgrumino &agruminoData);
String isAgruminoAttachedToUSB(DatiDeiSensoriDiAgrumino &agruminoData);
String isAgruminoBatteryCharging(DatiDeiSensoriDiAgrumino &agruminoData);
String booleanToString(boolean value, String trueValue, String falseValue);

#endif
