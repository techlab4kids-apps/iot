#include "ADS1115.h"

#ifdef SOFTWAREWIRE
    #include <SoftwareWire.h>
    SoftwareWire myWire(3, 2);
    ADS1115<SoftwareWire> ads(myWire);//IIC
#else
    #include <Wire.h>
    ADS1115<TwoWire> ads(Wire);//IIC
#endif

void setup(void) 
{
    Serial.begin(115200);
    while(!ads.begin(0x48)){
        Serial.print("ads1115 init failed!!!");
        delay(1000);
    }
    //ads.begin(0x49) 
    //ads.begin(0x4A)
    //ads.begin(0x4B)

    ads.setOperateMode(ADS1115_MODE_SINGLE);   
    ads.setOperateStaus(ADS1115_OS_SINGLE);

    ads.setPGAGain(ADS1115_PGA_6_144);    // 2/3x gain +/- 6.144V  1 bit =  0.1875mV (default)
    // ads.setPGAGain(ADS1115_PGA_4_096); // 1x gain   +/- 4.096V  1 bit =  0.125mV
    // ads.setPGAGain(ADS1115_PGA_2_048); // 2x gain   +/- 2.048V  1 bit =  0.0625mV
    // ads.setPGAGain(ADS1115_PGA_1_024); // 4x gain   +/- 1.024V  1 bit =  0.03125mV
    // ads.setPGAGain(ADS1115_PGA_0_512); // 8x gain   +/- 0.512V  1 bit =  0.015625mV
    // ads.setPGAGain(ADS1115_PGA_0_256); // 16x gain  +/- 0.256V  1 bit =  0.0078125mV
    
    // ads.setSampleRate(ADS1115_DR_8); //8 SPS
    // ads.setSampleRate(ADS1115_DR_16); //16 SPS
    // ads.setSampleRate(ADS1115_DR_32); //32 SPS
    // ads.setSampleRate(ADS1115_DR_64); //64 SPS
    // ads.setSampleRate(ADS1115_DR_128); //128 SPS
    // ads.setSampleRate(ADS1115_DR_250); //250 SPS
    // ads.setSampleRate(ADS1115_DR_475); //475 SPS
    ads.setSampleRate(ADS1115_DR_860); //860 SPS
}

void loop(void) 
{   
    int16_t adc01,adc03,adc13,adc23;
    adc01 = ads.getConversionResults(channel01); //P = AIN0, N = AIN1
    adc03 = ads.getConversionResults(channel03); //P = AIN0, N = AIN3
    adc13 = ads.getConversionResults(channel13); //P = AIN1, N = AIN3
    adc23 = ads.getConversionResults(channel23); //P = AIN2, N = AIN3
    Serial.print("P AIN0- N AIN1: "); Serial.println(adc01);
    Serial.print("P AIN0- N AIN3: "); Serial.println(adc03);
    Serial.print("P AIN1- N AIN3: "); Serial.println(adc13);
    Serial.print("P AIN2- N AIN3: "); Serial.println(adc23);
    Serial.println(" ");    
    delay(1000);
}