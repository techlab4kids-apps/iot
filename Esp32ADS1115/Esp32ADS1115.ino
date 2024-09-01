/* Barometer demo V1.0
*  Based largely on code by  Jim Lindblom
*  Get pressure, altitude, and temperature from the BMP085.
*  Serial.print it out at 9600 baud to serial monitor.
*
*  By:https://www.seeedstudio.com
*/
#include "Barometer.h"
#include <Wire.h>

float temperature;
float pressure;
float atm;
float altitude;
Barometer myBarometer;

void setup(){
    Serial.begin(115200);
    myBarometer.init();
}

void loop()
{
    temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
    pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());//Get the temperature
    altitude = myBarometer.calcAltitude(pressure); //Uncompensated calculation - in Meters
    atm = pressure / 101325;

    Serial.print("Temperature: ");
    Serial.print(temperature, 2); //display 2 decimal places
    Serial.println("deg C");

    Serial.print("Pressure: ");
    Serial.print(pressure, 0); //whole number only.
    Serial.println(" Pa");

    Serial.print("Ralated Atmosphere: ");
    Serial.println(atm, 4); //display 4 decimal places

    Serial.print("Altitude: ");
    Serial.print(altitude, 2); //display 2 decimal places
    Serial.println(" m");

    Serial.println();

    delay(1000); //wait a second and get values again.
}


//#include "ADS1115.h"
//
//#ifdef SOFTWAREWIRE
//    #include <SoftwareWire.h>
//    SoftwareWire myWire(3, 2);
//    ADS1115<SoftwareWire> ads(myWire);//IIC
//#else
//    #include <Wire.h>
//    ADS1115<TwoWire> ads(Wire);//IIC
//#endif
//
//void setup(void)
//{
//    Serial.begin(115200);
//    while(!ads.begin(0x48)){
//        Serial.print("ads1115 init failed!!!");
//        delay(1000);
//    }
//    //ads.begin(0x49)
//    //ads.begin(0x4A)
//    //ads.begin(0x4B)
//
//    ads.setOperateMode(ADS1115_OS_SINGLE);
//    ads.setOperateStaus(ADS1115_MODE_SINGLE);
//
//  ads.setPGAGain(ADS1115_PGA_6_144);    // 2/3x gain +/- 6.144V  1 bit =  0.1875mV (default)
//    // ads.setPGAGain(ADS1115_PGA_4_096); // 1x gain   +/- 4.096V  1 bit =  0.125mV
//    // ads.setPGAGain(ADS1115_PGA_2_048); // 2x gain   +/- 2.048V  1 bit =  0.0625mV
//    // ads.setPGAGain(ADS1115_PGA_1_024); // 4x gain   +/- 1.024V  1 bit =  0.03125mV
//    // ads.setPGAGain(ADS1115_PGA_0_512); // 8x gain   +/- 0.512V  1 bit =  0.015625mV
//    // ads.setPGAGain(ADS1115_PGA_0_256); // 16x gain  +/- 0.256V  1 bit =  0.0078125mV
//
//    ads.setSampleRate(ADS1115_DR_8); //8 SPS
//    // ads.setSampleRate(ADS1115_DR_16); //16 SPS
//    // ads.setSampleRate(ADS1115_DR_32); //32 SPS
//    // ads.setSampleRate(ADS1115_DR_64); //64 SPS
//    // ads.setSampleRate(ADS1115_DR_128); //128 SPS
//    // ads.setSampleRate(ADS1115_DR_250); //250 SPS
//    // ads.setSampleRate(ADS1115_DR_475); //475 SPS
//    // ads.setSampleRate(ADS1115_DR_860); //860 SPS
//}
//void loop(void)
//{
//    int16_t adc0,adc1,adc2,adc3;
//    adc0 = ads.getConversionResults(channel0); //P = AIN0, N = GND
//    adc1 = ads.getConversionResults(channel1); //P = AIN1, N = GND
//    adc2 = ads.getConversionResults(channel2); //P = AIN2, N = GND
//    adc3 = ads.getConversionResults(channel3); //P = AIN3, N = GND
//    Serial.print("AIN0: "); Serial.println(adc0);
//    Serial.print("AIN1: "); Serial.println(adc1);
//    Serial.print("AIN2: "); Serial.println(adc2);
//    Serial.print("AIN3: "); Serial.println(adc3);
//    Serial.println(" ");
//    delay(1000);
//}
