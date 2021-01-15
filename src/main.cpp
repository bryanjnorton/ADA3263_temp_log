//Temp Log
#include <Arduino.h>
#include "CmdMessenger.h"
#include <Adafruit_MAX31856.h>

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(6, 7, 8, 9);

// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

enum {
    who_are_you,
    my_name_is,
    get_probe_temp,
    temp_is,
    get_cold_junction_temp,
    error,
};

CmdMessenger c = CmdMessenger(Serial,',',';','/');

void on_who_are_you(void){
    c.sendCmd(my_name_is,"Temp logger");
}

void on_get_probe_temp(void){

  c.sendCmdStart(temp_is);
  c.sendCmdArg("Probe Temp (C):");
  c.sendCmdBinArg(maxthermo.readThermocoupleTemperature());
  c.sendCmdEnd();

}

void on_get_cold_junction_temp(void){

  c.sendCmdStart(temp_is);
  c.sendCmdArg("Cold Junction Temp (C):");
  c.sendCmdBinArg(maxthermo.readCJTemperature());
  c.sendCmdEnd();

}

void on_unknown_command(void){
  c.sendCmd(error,"Command without callback.");
}

/* Attach callbacks for CmdMessenger commands */
void attach_callbacks(void) { 
    c.attach(who_are_you,on_who_are_you);
    c.attach(get_probe_temp,on_get_probe_temp);
    c.attach(get_cold_junction_temp,on_get_cold_junction_temp);
    c.attach(on_unknown_command);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial.println("MAX31856 thermocouple test");

  maxthermo.begin();

  maxthermo.setThermocoupleType(MAX31856_TCTYPE_K);

  Serial.print("Thermocouple type: ");
  switch (maxthermo.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
    case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }

  attach_callbacks();    

}

// void loop() {
//   Serial.print("Cold Junction Temp: ");
//   Serial.println(maxthermo.readCJTemperature());

//   Serial.print("Thermocouple Temp: ");
//   Serial.println(maxthermo.readThermocoupleTemperature());
//   // Check and print any faults
//   uint8_t fault = maxthermo.readFault();
//   if (fault) {
//     if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
//     if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
//     if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
//     if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
//     if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
//     if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
//     if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
//     if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
//   }
//   delay(1000);
// }

void loop() {
  c.feedinSerialData();
}