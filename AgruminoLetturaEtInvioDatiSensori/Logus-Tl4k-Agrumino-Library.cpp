#include "Logus-Tl4k-Agrumino-Library.h"

extern Agrumino agrumino;


void accendiAgrumino(){
  agrumino.setup();
  agrumino.turnBoardOn();

  //Wire.begin(0, 2);

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

void toggleLed()
{
  if (agrumino.isLedOn())
  {
    Serial.println("Switching led OFF");
    agrumino.turnLedOff();
  }
  else
  {
    Serial.println("Switching led ON");
    agrumino.turnLedOn();
  }
}

void setLedStatus(bool status){
  if(status){
    agrumino.turnLedOn();
  }
  else{
    agrumino.turnLedOff();
  }
}

void blinkLed(int duration, int blinks)
{
  Serial.println("Start led blinking");
  for (int i = 0; i < blinks; i++)
  {
    agrumino.turnLedOn();
    delay(duration);
    agrumino.turnLedOff();
    if (i < blinks)
    {
      delay(duration); // Avoid delay in the latest loop ;)
    }
  }
  Serial.println("Led blinking ended");
}

const String getChipId()
{
  // Returns the ESP Chip ID, Typical 7 digits
  return String(ESP.getChipId());
}


// If the Agrumino S1 button is pressed for 5 seconds then reset the wifi saved settings.
boolean checkIfResetWiFiSettings()
{

  int remainingsLoops = (5 * 1000) / 100;

  Serial.print("Check if reset WiFi settings: ");

  while (agrumino.isButtonPressed() && remainingsLoops > 0)
  {
    delay(100);
    remainingsLoops--;
    Serial.print(".");
  }

  if (remainingsLoops == 0)
  {
    // Reset Wifi Settings
    Serial.println(" YES!");
    return true;
  }
  else
  {
    Serial.println(" NO");
    return false;
  }
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
