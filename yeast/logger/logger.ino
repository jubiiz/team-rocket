#include <Wire.h>
#include <SPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_CCS811.h>
#include <SD.h>
File file;

//for faster prints
#define pr(msg) Serial.print(msg)
#define prln(msg) Serial.println(msg)
#define USE_SD false

//if you need to read altitude,you need to know the sea level pressure
#define SEALEVELPRESSURE_HPA (1013.25)

//This Macro definition decide whether you use I2C or SPI
//When USEIIC is 1 means use I2C interface, When it is 0,use SPI interface

Adafruit_BME280 bme;
Adafruit_CCS811 ccs;


unsigned long delayTime = 5000;
long initial_time = millis();



void setup() {

    pinMode(LED_BUILTIN, OUTPUT);


    Serial.begin(9600);
    bool rslt;
    rslt = bme.begin();  
    if (!rslt) {
        Serial.println("Init Fail,Please Check your address or the wire you connected!!!");
        while (1);
    }
  
    Serial.println("bme Init Success");
    digitalWrite(LED_BUILTIN, HIGH);

    // using the ccs811 module
    if(!ccs.begin()){
      Serial.println("Failed to start sensor! Please check your wiring.");
      while(1);
    }

    // Wait for the sensor to be ready
    while(!ccs.available());
    Serial.println("ccs settup, trying SD");
    digitalWrite(LED_BUILTIN, LOW);
    
    if(true){
      if (!SD.begin(10)){
        Serial.println("sd failure");
        while(1);
      }
      file = SD.open("yeast.txt", FILE_WRITE);
      if(file){
        Serial.println("success file");
        file.println("------------NEW READING------------");
      }
      else{
        Serial.print("file failure");
        while(1);
      }
    }
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if(millis()-initial_time > 720000){
    file.close();
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.print("serial closed");
    while(1){
      Serial.println("done");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }
   if(ccs.available()){
    if(!ccs.readData()){
      // put your main code here, to run repeatedly:
      float pressure = bme.readPressure()/100.0F;
      float temperature = bme.readTemperature();
      float humidity = bme.readHumidity();
      float co2 = ccs.geteCO2();
      float tvoc = ccs.getTVOC();
    
      //log data to sd card
      file.print(pressure);
      file.print(",");
      file.print(temperature);
      file.print(",");
      file.print(humidity);
      file.print(",");
      file.print(co2);
      file.print(",");
      file.println(tvoc);
    
      // print to 
      Serial.print(pressure);
      Serial.print("temp, ");
      Serial.print(temperature);
      Serial.print("humidity, ");
      Serial.print(humidity);
      Serial.print("co2, ");
      Serial.print(co2);
      Serial.print("tvoc, ");
      Serial.println(tvoc);
   }
  }
  

  delay(delayTime);
}
