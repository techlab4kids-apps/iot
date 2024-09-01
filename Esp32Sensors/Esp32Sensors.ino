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

#include "seeed_bme680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define IIC_ADDR  uint8_t(0x76)

Seeed_BME680 bme680(IIC_ADDR); /* IIC PROTOCOL */

#define AS0 32
#define AS1 33
#define AS2 34
#define AS3 35
#define AS4 36

#include "sensirion_common.h"
#include "sgp30.h"

#include "ADS1115.h"

#ifdef SOFTWAREWIRE
    #include <SoftwareWire.h>
    SoftwareWire myWire(3, 2);
    ADS1115<SoftwareWire> ads(myWire);//IIC
#else
    #include <Wire.h>
    ADS1115<TwoWire> ads(Wire);//IIC
#endif

#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

U8G2_SH1107_SEEED_128X128_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);


void setup(){
    Serial.begin(115200);

    while(!ads.begin(0x48)){
        Serial.print("ads1115 init failed!!!");
        delay(1000);
    }

    u8g2.begin();
      u8g2.enableUTF8Print();
    
//    myBarometer.init();
//
//    while (!bme680.init()) {
//        Serial.println("bme680 init failed ! can't find device!");
//        delay(10000);
//    }
//
//    s16 err;
//    u32 ah = 0;
//    u16 scaled_ethanol_signal, scaled_h2_signal;
//
//    /*  Init module,Reset all baseline,The initialization takes up to around 15 seconds, during which
//        all APIs measuring IAQ(Indoor air quality ) output will not change.Default value is 400(ppm) for co2,0(ppb) for tvoc*/
//    while (sgp_probe() != STATUS_OK) {
//        Serial.println("SGP failed");
//        while (1);
//    }
//    /*Read H2 and Ethanol signal in the way of blocking*/
//    err = sgp_measure_signals_blocking_read(&scaled_ethanol_signal,
//                                            &scaled_h2_signal);
//    if (err == STATUS_OK) {
//        Serial.println("get ram signal!");
//    } else {
//        Serial.println("error reading signals");
//    }
//
//    //ah=get_relative_humidity();
//    /*
//        The function sgp_set_absolute_humidity() need your own implementation
//    */
//    //sgp_set_absolute_humidity(ah);
//
//    // Set absolute humidity to 13.000 g/m^3
//    //It's just a test value
//    sgp_set_absolute_humidity(12285);
//    err = sgp_iaq_init();

}

void loop()
{
//    Serial.print("New measures!");
//    Serial.println("");
//    
//    temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT()); //Get the temperature, bmp085ReadUT MUST be called first
//    pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());//Get the temperature
//    altitude = myBarometer.calcAltitude(pressure); //Uncompensated calculation - in Meters
//    atm = pressure / 101325;
//
//    Serial.print("Temperature: ");
//    Serial.print(temperature, 2); //display 2 decimal places
//    Serial.println("deg C");
//
//    Serial.print("Pressure: ");
//    Serial.print(pressure, 0); //whole number only.
//    Serial.println(" Pa");
//
//    Serial.print("Ralated Atmosphere: ");
//    Serial.println(atm, 4); //display 4 decimal places
//
//    Serial.print("Altitude: ");
//    Serial.print(altitude, 2); //display 2 decimal places
//    Serial.println(" m");
//
//    if (bme680.read_sensor_data()) {
//        Serial.println("Failed to perform reading :(");
//        return;
//    }
//    Serial.print("temperature ===>> ");
//    Serial.print(bme680.sensor_result_value.temperature);
//    Serial.println(" C");
//
//    Serial.print("pressure ===>> ");
//    Serial.print(bme680.sensor_result_value.pressure / 1000.0);
//    Serial.println(" KPa");
//
//    Serial.print("humidity ===>> ");
//    Serial.print(bme680.sensor_result_value.humidity);
//    Serial.println(" %");
//
//    Serial.print("gas ===>> ");
//    Serial.print(bme680.sensor_result_value.gas / 1000.0);
//    Serial.println(" Kohms");
//
//    Serial.println();
//
//    s16 err = 0;
//    u16 tvoc_ppb, co2_eq_ppm;
//    err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
//    if (err == STATUS_OK) {
//        Serial.print("tVOC  Concentration:");
//        Serial.print(tvoc_ppb);
//        Serial.println("ppb");
//
//        Serial.print("CO2eq Concentration:");
//        Serial.print(co2_eq_ppm);
//        Serial.println("ppm");
//    } else {
//        Serial.println("error reading IAQ values\n");
//    }
//
//    int luminosityValue1 = analogRead(AS0);
//    Serial.print("Luminosity 1: ");
//    Serial.println(luminosityValue1);
//
//    int luminosityValue2 = analogRead(AS1);
//    Serial.print("Luminosity 2: ");
//    Serial.println(luminosityValue2);
////
//    int uvValue = analogRead(AS2);
//    Serial.print("UV value: ");
//    Serial.println(uvValue);

    int16_t adc0,adc1,adc2,adc3;
    adc0 = ads.getConversionResults(channel0); //P = AIN0, N = GND
    Serial.print("Intensità della corrente: "); Serial.println(adc0);


    u8g2.firstPage();

    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.print("Corrente");
    //      u8g2.drawStr(0,24,"Corrente:");
    u8g2.print(adc0);
//    do {
//      u8g2.setFont(u8g2_font_ncenB10_tr);
//      u8g2.print("Corrente);
////      u8g2.drawStr(0,24,"Corrente:");
//      u8g2.print(adc0);
//    } while ( u8g2.nextPage() );

    delay(1000); //wait a second and get values again.

     Serial.println();
     Serial.println();
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
