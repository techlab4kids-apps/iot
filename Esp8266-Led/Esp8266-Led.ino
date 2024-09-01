// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <EasyButton.h>
// At the global level
#define FLASH_PIN 0

EasyButton resetButton(FLASH_PIN);
#define RESET_DURATION 5000

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include "Adafruit_NeoPixel.h"
#ifdef __AVR__
    #include <avr/power.h>
#endif

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <mDNSResolver.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <Hash.h>


ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

#define USE_SERIAL Serial1

#include "Ultrasonic.h"

#include <Wire.h>
#include <AS5600.h>

AMS_5600 ams5600;

#include "LIS3DHTR.h"
LIS3DHTR<TwoWire> LIS; //IIC
#define WIRE Wire

WiFiManager wifiManager;

int ang, lang = 0;

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            D6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      10

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Ultrasonic ultrasonic(D7);

int delayval = 500; // delay for half a second

#include <ESP8266WiFi.h>

#include <ESP8266mDNS.h>

using namespace mDNSResolver;
WiFiClient wifiClient;
WiFiUDP udp;
Resolver resolver(udp);

void printTimestamp(Print *_logOutput)
{

    // Division constants
    const unsigned long MSECS_PER_SEC = 1000;
    const unsigned long SECS_PER_MIN = 60;
    const unsigned long SECS_PER_HOUR = 3600;
    const unsigned long SECS_PER_DAY = 86400;

    // Total time
    const unsigned long msecs = millis();
    const unsigned long secs = msecs / MSECS_PER_SEC;

    // Time in components
    const unsigned long MilliSeconds = msecs % MSECS_PER_SEC;
    const unsigned long Seconds = secs % SECS_PER_MIN;
    const unsigned long Minutes = (secs / SECS_PER_MIN) % SECS_PER_MIN;
    const unsigned long Hours = (secs % SECS_PER_DAY) / SECS_PER_HOUR;

    // Time as string
    char timestamp[20];
    sprintf(timestamp, "%02d:%02d:%02d.%03d ", (int)Hours, (int)Minutes, (int)Seconds, (int)MilliSeconds);
    _logOutput->print(timestamp);
}

void printLogLevel(Print *_logOutput, int logLevel)
{
    /// Show log description based on log level
    switch (logLevel)
    {
    default:
    case 0:
        _logOutput->print("SILENT\t");
        break;
    case 1:
        _logOutput->print("FATAL\t");
        break;
    case 2:
        _logOutput->print("ERROR\t");
        break;
    case 3:
        _logOutput->print("WARNING\t");
        break;
    case 4:
        _logOutput->print("INFO\t");
        break;
    case 5:
        _logOutput->print("TRACE\t");
        break;
    case 6:
        _logOutput->print("VERBOSE\t");
        break;
    }
}

void printPrefix(Print *_logOutput, int logLevel)
{
    printTimestamp(_logOutput);
    printLogLevel(_logOutput, logLevel);
}

void printSuffix(Print *_logOutput, int logLevel)
{
    _logOutput->print("");
}



void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case sIOtype_DISCONNECT:
            USE_SERIAL.printf("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

            // join default namespace (no auto join in Socket.IO V3)
            socketIO.send(sIOtype_CONNECT, "/");
            break;
        case sIOtype_EVENT:
            USE_SERIAL.printf("[IOc] get event: %s\n", payload);
            break;
        case sIOtype_ACK:
            USE_SERIAL.printf("[IOc] get ack: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_ERROR:
            USE_SERIAL.printf("[IOc] get error: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_EVENT:
            USE_SERIAL.printf("[IOc] get binary: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_ACK:
            USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
            hexdump(payload, length);
            break;
    }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Setup started");

    resetButton.begin();
    // Allows me to set a custom duration for how long a user holds the reset button for.
    // If you want it to immediately reset, just use `resetButton.onPressed(reset);`
    // Here, `reset` is the `void reset()` function defined below. 
    // We're passing the callback function, which is called when the library detects the button is pressed for DURATION amount of ms. 
    resetButton.onPressedFor(RESET_DURATION, reset);
  
    String deviceId = "tl4k-esp8266-" + String(ESP.getChipId());
    Serial.println("Device ID: " + deviceId);

    wifiManager.setAPCallback(configModeCallback);
    //first parameter is name of access point, second is the password
    wifiManager.autoConnect(deviceId.c_str(), "techlab4kids");

    // Wait for connect to AP
    int tries = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      tries++;
      if (tries > 30) {
        break;
      }
    }
    printWifiStatus();
    Serial.println();
    String serverName = "FLS-SERVER.local";

    IPAddress ip = resolver.search(serverName.c_str());
    Serial.println("Sending mDNS query");

    String serverIp = ip.toString();
    Serial.println("serverIp: " + serverIp);

    // server address, port and URL
    socketIO.begin(serverIp.c_str(), 3005, "");

    // event handler
    socketIO.onEvent(socketIOEvent);

  
    pixels.setBrightness(255);
    pixels.begin(); // This initializes the NeoPixel library.

    LIS.begin(WIRE,0x19); //IIC init
    //LIS.begin(0x19);
    LIS.openTemp();  //If ADC3 is used, the temperature detection needs to be turned off.
    //  LIS.closeTemp();//default
    delay(100);
    LIS.setFullScaleRange(LIS3DHTR_RANGE_2G);
  //  LIS.setFullScaleRange(LIS3DHTR_RANGE_4G);
  //  LIS.setFullScaleRange(LIS3DHTR_RANGE_8G);
  //  LIS.setFullScaleRange(LIS3DHTR_RANGE_16G);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_1HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_10HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_25HZ);
  LIS.setOutputDataRate(LIS3DHTR_DATARATE_50HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_100HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_200HZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_1_6KHZ);
  //  LIS.setOutputDataRate(LIS3DHTR_DATARATE_5KHZ);

  Wire.begin();
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>> ");
  /*
  if(ams5600.detectMagnet() == 0 ){
    while(1){
        if(ams5600.detectMagnet() == 1 ){
            Serial.print("Current Magnitude: ");
            Serial.println(ams5600.getMagnitude());
            break;
        }
        else{
            Serial.println("Can not detect magnet");
        }
        delay(1000);
    }
  }
  */  

  Serial.println(); 
}

/*******************************************************
/* Function: convertRawAngleToDegrees
/* In: angle data from AMS_5600::getRawAngle
/* Out: human readable degrees as float
/* Description: takes the raw angle and calculates
/* float value in degrees.
/*******************************************************/
float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087 of a degree */
  float retVal = newAngle * 0.087;
  ang = retVal;
  return ang;
}

void setPixelsColor(int currentDistance, int top, int bottom, int R, int G, int B, int revert){
  int topPixelIndex;
/*
    Serial.print("Top: ");
    Serial.println(top);

    Serial.print("Bottom: ");
    Serial.println(bottom);

    Serial.print("CurrentDistance: ");
    Serial.println(currentDistance);
*/

  if (currentDistance > bottom && currentDistance <= top){
      pixels.clear();

      topPixelIndex = top - currentDistance +1;

      Serial.print("TopPixelIndex: ");
      Serial.println(topPixelIndex);
      
      if (topPixelIndex > 10){
        topPixelIndex = 10;
      }

      if (revert){
          for (int i = 0; i < NUMPIXELS - topPixelIndex; i++) {
          pixels.setPixelColor(i, pixels.Color(R, G, B));
        }
      } else{
        for (int i = 0; i < topPixelIndex; i++) {
          pixels.setPixelColor(i, pixels.Color(R, G, B));
        }
      }
    }
}

void setPixelsFullColor(int R, int G, int B){

    pixels.clear();

    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(R, G, B));
    }
}


void reset() {
  Serial.println("FLASH was held for " + String(RESET_DURATION, DEC) + "ms.");
  Serial.println("Erasing stored WiFi credentials.");
  
  // clear WiFi creds.
  WiFiManager wifiManager;
  wifiManager.resetSettings();
   
  Serial.println("Restarting...");
  ESP.restart(); // builtin, safely restarts the ESP. 
}


void loop() {
    Serial.println();
    Serial.println("########### Looping ###########");
    Serial.println();
    
    resetButton.read();

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Wifi still disconnected");
    } else {
      Serial.println("Wifi Connected");
    }

    long rangeInCentimeters;

    Serial.println("The distance to obstacles in front is: ");

    rangeInCentimeters = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
    if (rangeInCentimeters >= 400){
      rangeInCentimeters = 0;  
    }
    Serial.print(rangeInCentimeters);//0~400cm
    Serial.println(" cm");

    Serial.print("Angle: ");
    Serial.println(String(convertRawAngleToDegrees(ams5600.getRawAngle()),DEC));

    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

      int top;
      int bottom;

      if (rangeInCentimeters > 30){
        
      }
      
      if (rangeInCentimeters > 20 && rangeInCentimeters <= 30){
          top = 30;
          bottom = 20;
          setPixelsColor(rangeInCentimeters, top, bottom, 0, 160, 0, 1);
        } else if (rangeInCentimeters > 10 && rangeInCentimeters <= 20){
          top = 20;
          bottom = 10;
          setPixelsColor(rangeInCentimeters, top, bottom, 160, 160, 0, 0);
        } else if (rangeInCentimeters > 0 && rangeInCentimeters <= 10){
          top = 10;
          bottom = 0;
          setPixelsColor(rangeInCentimeters, top, bottom, 160, 0, 0, 0);
        }

  if (!LIS)
  {
    Serial.println("LIS3DHTR didn't connect.");
    while (1)
      ;
    return;
  }
  //3 axis
  //    Serial.print("x:"); Serial.print(LIS.getAccelerationX()); Serial.print("  ");
  //    Serial.print("y:"); Serial.print(LIS.getAccelerationY()); Serial.print("  ");
  //    Serial.print("z:"); Serial.println(LIS.getAccelerationZ());
  //ADC
  //    Serial.print("adc1:"); Serial.println(LIS.readbitADC1());
  //    Serial.print("adc2:"); Serial.println(LIS.readbitADC2());
  //    Serial.print("adc3:"); Serial.println(LIS.readbitADC3());

  //temperature
  Serial.print("temp:");
  Serial.println(LIS.getTemperature());        
  
  
  
  pixels.show(); // This sends the updated pixel color to the hardware.
  
  //delay(delayval); // Delay for a period of time (in milliseconds).
 
  
  delay(500);

}
