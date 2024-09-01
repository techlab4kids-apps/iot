
/////////////////////
// Utility methods //
/////////////////////

#include "Logus-Tl4k-Agrumino-Library.h"

extern Agrumino agrumino;

void accendiAgrumino(){
  agrumino.turnBoardOn();
}

void impostaAgruminoAlRisveglio(){
  Serial.begin(115200);
  agrumino.setup();
}

void stampaUnTestoSuUnaRigaDelMonitorSeriale(const char * testo){
  Serial.println(testo);
}

void stampaUnaRigaVuotaSulMonitorSeriale(){
  stampaUnTestoSuUnaRigaDelMonitorSeriale("\n");
}

// Agrumino utilities

void leggiDatiDaiSensoriDiAgrumino(DatiDeiSensoriDiAgrumino &agruminoData){
  getAgruminoData(agruminoData);
}
void stampaDatiDaAgruminoSulMonitorSeriale(DatiDeiSensoriDiAgrumino &agruminoData){
   printAgruminoData(agruminoData);
}

void getAgruminoData(DatiDeiSensoriDiAgrumino &agruminoData)
{
  agruminoData.temperature = agruminoGetTemperature();
  agruminoData.soilMoisture = agrumino.readSoil();
  agruminoData.illuminance = agrumino.readLux();
  agruminoData.batteryVoltage = agrumino.readBatteryVoltage();
  agruminoData.batteryLevel = agrumino.readBatteryLevel();
  agruminoData.isAttachedToUSB = agrumino.isAttachedToUSB();
  agruminoData.isBatteryCharging = agrumino.isBatteryCharging();
  agruminoData.isLedOn = agrumino.isLedOn();
}

void printAgruminoData(DatiDeiSensoriDiAgrumino &agruminoData)
{
  Serial.println("Dati letti dai sensori...");
  Serial.println("temperatura:          " + String(agruminoData.temperature) + "°C");
  Serial.println("umidità del terreno:  " + String(agruminoData.soilMoisture) + "%");
  Serial.println("luminosità:           " + String(agruminoData.illuminance) + " lux");
  Serial.println("voltaggio batteria:   " + String(agruminoData.batteryVoltage) + " V");
  Serial.println("livello batteria:     " + String(agruminoData.batteryLevel) + "%");
  Serial.println("connessione USB:      " + isAgruminoAttachedToUSB(agruminoData));
  Serial.println("batteria in carica:   " + isAgruminoBatteryCharging(agruminoData));
  Serial.println("led acceso:           " + isLedOn(agruminoData));
  Serial.println();
}

String isLedOn(DatiDeiSensoriDiAgrumino &agruminoData){
  return booleanToString(agruminoData.isLedOn, "si", "no");
}

String isAgruminoAttachedToUSB(DatiDeiSensoriDiAgrumino &agruminoData){
  return booleanToString(agruminoData.isAttachedToUSB, "si", "no");
}

String isAgruminoBatteryCharging(DatiDeiSensoriDiAgrumino &agruminoData){
  return booleanToString(agruminoData.isBatteryCharging, "si", "no");
}

String booleanToString(boolean value, String trueValue, String falseValue){
    if(value){
    return trueValue;
  }
  return falseValue;
}

float agruminoGetTemperature()
{
  float temperature = agrumino.readTempC();
  boolean isAttachedToUSB = agrumino.isAttachedToUSB();
  if (isAttachedToUSB)
  {
    return temperature - CORREZIONE_DELLA_TEMPERATURA;
  }
  else
  {
    return temperature;
  }
}

void faiLampeggiareIlLed(){
  blinkLed() ;
}

void blinkLed() {
  agrumino.turnLedOn();
  delay(200);
  agrumino.turnLedOff();
}

void addormentaProfondamenteAgrumino(){
  agrumino.turnBoardOff(); // Board off before delay/sleep to save battery :)

  // delaySec(SLEEP_TIME_SEC); // The ESP8266 stays powered, executes the loop
  // repeatedly
  deepSleepSec(
      DURATA_DEL_SONNO_IN_SEC); // ESP8266 enter in deepSleep and after the selected time
                       // starts back from setup() and then loop()
}

void addormentaAgrumino(){
  agrumino.turnBoardOff(); // Board off before delay/sleep to save battery :)

  delaySec(DURATA_DEL_SONNO_IN_SEC); // The ESP8266 stays powered, executes the loop
}

void delaySec(int sec) { 
  delay(sec * 1000); 
}

void deepSleepSec(int sec) {
  ESP.deepSleep(sec * 1000000); // microseconds
}

void accendiLaPompaSePulsantePremuto(){
  //  if (isButtonPressed) {
//    agrumino.turnWateringOn();
//    delay(2000);
//    agrumino.turnWateringOff();
//  }
}
