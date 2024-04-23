#ifndef TL4K_LIBRARY_H
#define TL4K_LIBRARY_H

#include "Arduino.h"

#include "Ultrasonic.h"

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include "Adafruit_NeoPixel.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define D6            6
#define D7            7

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            D6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      10

extern Adafruit_NeoPixel strip;

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

void impostaStrisciaLed(int valore);
void setStrisciaLed(int value);

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

// Led BAR

void colorWipe(uint32_t color, int wait);
void whiteOverRainbow(int whiteSpeed, int whiteLength);
void pulseWhite(uint8_t wait);
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops);
void knightRider(uint32_t color, int times, int wait, int barLength);

// END led Bar

#endif
