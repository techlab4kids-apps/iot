
#include "Tl4k-ESP8266-Library.h"

Ultrasonic ultrasonic(D7);

bool isBuiltinLedOn;
// EasyButton resetButton(D3);

long leggiLaDistanza(){
  return readDistance();  
}

long readDistance(){
  return ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
}

void configuraSchedaEtSensori(){
  //resetButton.begin();

  isBuiltinLedOn = false;
  //pinMode(D0, OUTPUT);

  strip.setBrightness(200);
  strip.begin(); // This initializes the NeoPixel library.

}

void stampaUnTestoSuUnaRigaDelMonitorSeriale(const char * testo){
  Serial.println(testo);
}

void stampaUnaRigaVuotaSulMonitorSeriale(){
  stampaUnTestoSuUnaRigaDelMonitorSeriale("\n");
}

void leggiDati(Dati &data){
  getData(data);
}
void stampaDatiSulMonitorSeriale(Dati &data){
   printData(data);
}

bool isLedOn(){
  return isBuiltinLedOn;
}

void getData(Dati &data)
{
  data.isLedOn = isLedOn();
}

void printData(Dati &data)
{
  Serial.println("Dati letti dai sensori...");
  // Serial.println("temperatura:          " + String(agruminoData.temperature) + "°C");

  // Serial.println("led acceso:           " + isLedOn(agruminoData));
  Serial.println();
}

String isLedOn(Dati &data){
  return booleanToString(data.isLedOn, "si", "no");
}

String booleanToString(boolean value, String trueValue, String falseValue){
    if(value){
    return trueValue;
  }
  return falseValue;
}

//float getTemperature()
//{
//  float temperature = agrumino.readTempC();
//  boolean isAttachedToUSB = agrumino.isAttachedToUSB();
//  if (isAttachedToUSB)
//  {
//    return temperature - CORREZIONE_DELLA_TEMPERATURA;
//  }
//  else
//  {
//    return temperature;
//  }
//}

void faiLampeggiareIlLed(){
  blinkLed() ;
}

void turnLedOn(){
//  digitalWrite(D0, HIGH);
  isBuiltinLedOn = true;  
}

void turnLedOff(){
//  digitalWrite(D0, LOW);
  isBuiltinLedOn = false;  
}

void blinkLed() {
  turnLedOn();
  delay(200);
  turnLedOff();
}

void toggleLed()
{
  if (isBuiltinLedOn)
  {
    Serial.println("Switching led OFF");
    turnLedOff();
  }
  else
  {
    Serial.println("Switching led ON");
    turnLedOn();
  }
}

void setLedStatus(bool status){
  if(status){
    turnLedOn();
  }
  else{
    turnLedOff();
  }
}

void blinkLed(int duration, int blinks)
{
  Serial.println("Start led blinking");
  for (int i = 0; i < blinks; i++)
  {
    turnLedOn();
    delay(duration);
    turnLedOff();
    if (i < blinks)
    {
      delay(duration); // Avoid delay in the latest loop ;)
    }
  }
  Serial.println("Led blinking ended");
}

void delaySec(int sec) { 
  delay(sec * 1000); 
}
