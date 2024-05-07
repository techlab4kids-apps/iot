
#include "Tl4k-LedStick-Library.h"


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, D6, NEO_GRB + NEO_KHZ800);

void impostaStrisciaLed(int valore){
  setStrisciaLed(valore);
}
void setStrisciaLed(int value){
  Serial.println("distanza:          " + String(value));

// for(int i=0;i<NUMPIXELS;i++){
//
//    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
//    pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
//
//    pixels.show(); // This sends the updated pixel color to the hardware.
//
//    delay(100); // Delay for a period of time (in milliseconds).
//
//  }

  for(int i = 0; i < NUMPIXELS; i++) {
    if (value > 10) {
      strip.setPixelColor(i, strip.Color(0, 255, 0)); // All LEDs green
    } else if (value > 5 && value <= 10) {
      if (i < NUMPIXELS / 2) {
        strip.setPixelColor(i, strip.Color(0, 255, 0)); // First half green
      } else {
        strip.setPixelColor(i, strip.Color(255, 255, 0)); // Second half yellow
      }
    } else if (value <= 1) {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); // All LEDs red
    }

     strip.show();
  }
}

// led bar

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
    delay(1000); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
  }

  delay(500); // Pause 1/2 second
}

void knightRider(uint32_t color, int times, int wait, int barLength) {

  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  uint32_t off = (0, 0, 0);

  int i=0;
  int j=0;
  int k=0;
  while(i<times) {
      for(int k=0; k<barLength; k++) {
       strip.setPixelColor(k, color);         //  Set pixel's color (in RAM)
       strip.show();                          //  Update strip to match
       delay(wait);                           //  Pause for a moment
      }
      for(int j=0; j<strip.numPixels(); j++) {
       strip.setPixelColor(j, off);         //  Set Bar Pixel 1 to 0 (in RAM)
       strip.setPixelColor(j+barLength, color);         //  Set next Bar pixel to color (in RAM)
       strip.show();                          //  Update strip to match
       delay(wait);
      }
      for(int k=strip.numPixels(); k>strip.numPixels()-barLength; k--) {
       strip.setPixelColor(k, color);         //  Set pixel's color (in RAM)
       strip.show();                          //  Update strip to match
       delay(wait);                           //  Pause for a moment
        }
      for(int j=strip.numPixels(); j>0; j--) {
       strip.setPixelColor(j, off);         //  Set Bar Pixel 1 to 0 (in RAM)
       strip.setPixelColor(j-barLength, color);         //  Set next Bar pixel to color (in RAM)
       strip.show();                          //  Update strip to match
       delay(wait);
       }
      strip.setPixelColor(0, off);
      strip.show();                          //  Update strip to match
      delay(wait);
      i++;
   }
}

// END led bar
