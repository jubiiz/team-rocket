#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_CCS811.h>



//if you need to read altitude,you need to know the sea level pressure
#define SEALEVELPRESSURE_HPA (1015.95)

Adafruit_BME280 bme;
Adafruit_CCS811 ccs;

unsigned long delayTime;
bool success = false;

void printEnvValues();
void printAQ();

void setup() {
    Serial.begin(9600);
  
    bool rslt;
    rslt = bme.begin();  
    if (!rslt) {
        Serial.println("BME280 Init Fail,Please Check your address or the wire you connected!!!");
        while (1);
    }  
    Serial.println("Bme280 Init Success");

   rslt = ccs.begin();
   if (!rslt) {
      Serial.println(" CCS811 Init Fail,Please Check your address or the wire you connected!!!");
      while(1);
   }
   Serial.println("CCS811 Init success");

    
    delayTime = 40;   
}

void loop() { 
    printEnvValues();
    printAQ();
    delay(1000);
}

void printAQ(){
  if(ccs.available()){
    if(!ccs.readData()){
      Serial.print(" CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.println(ccs.getTVOC());
      success = true;
    } 
    else{
      success = false; 
    }
  }
  else{
    success = false;
  }
}

void printEnvValues() {
  if(success){
    Serial.print("temperature:");
    Serial.print(bme.readTemperature());
    Serial.print("*C   ");

    Serial.print("pressure:");
    Serial.print(bme.readPressure()/100.0F);
    Serial.print("hPa   ");
  
    Serial.print("altitude:");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.print("m");
  }
}
