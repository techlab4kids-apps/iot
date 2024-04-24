#ifndef TL4K_LIBRARY_H
#define TL4K_LIBRARY_H

#include "Arduino.h"

#include "Tl4k-LedStick-Library.h"

#include "Ultrasonic.h"



// #include <EasyButton.h>

//#define FLASH_PIN 0
// extern EasyButton resetButton;

#define RESET_DURATION 5000

typedef struct Data_t
{
  float temperature;
  unsigned int soilMoisture;
  float illuminance;
  float batteryVoltage;
  unsigned int batteryLevel;
  bool isAttachedToUSB;
  bool isBatteryCharging;
  bool isLedOn;
  String ledColor;
} Dati;

void configuraSchedaEtSensori();

long readDistance();
long leggiLaDistanza();

void stampaUnaRigaVuotaSulMonitorSeriale();
void stampaUnTestoSuUnaRigaDelMonitorSeriale(const char * testo);

void leggiDati(Dati &data);
void stampaDatiSulMonitorSeriale(Dati &adata);

void getData(Dati &data);
void printData(Dati &data);

float getTemperature();

void faiLampeggiareIlLed();
void blinkLed();

void blinkLed(int duration, int blinks);

void toggleLed();

void setLedStatus(bool status);

const String getChipId();

void delaySec(int sec);
void deepSleepSec(int sec);

String isLedOn(Dati &data);
bool isLedOn();
String booleanToString(bool value, String trueValue, String falseValue);

#endif
