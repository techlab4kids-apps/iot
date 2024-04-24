#ifndef TL4K_LEDSTICK_LIBRARY_H
#define TL4K_LEDSTICK_LIBRARY_H

#include "Arduino.h"

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include "Adafruit_NeoPixel.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            D6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      10

extern Adafruit_NeoPixel strip;

void impostaStrisciaLed(int valore);
void setStrisciaLed(int value);

// Led BAR

void colorWipe(uint32_t color, int wait);
void whiteOverRainbow(int whiteSpeed, int whiteLength);
void pulseWhite(uint8_t wait);
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops);
void knightRider(uint32_t color, int times, int wait, int barLength);

// END led Bar

#endif
