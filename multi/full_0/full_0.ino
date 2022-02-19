#include <Wire.h>
#include <SPI.h>
#include "helper.h"


#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_CCS811.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define CD_UPDATE_GAP 3000

#define LEDPIN 2

Adafruit_BME280 bme;
Adafruit_CCS811 ccs;

//setting up chute update
int *pcounter;
float *psum;
float *pstart_time;
float *plast_alt;

void setup() {
    Serial.begin(9600);
    pinMode(LEDPIN, OUTPUT); 
    
    // start BME sensor
    bool rslt;
    rslt = bme.begin();  
    if (!rslt) {
        Serial.println("BME280 Init Fail,Please Check your address or the wire you connected!!!");
        while (1);
    }  
    Serial.println("BME280 Init Success");

    // start ccs811 sensor
    rslt = ccs.begin();
    if (!rslt) {
      Serial.println(" CCS811 Init Fail,Please Check your address or the wire you connected!!!");
      while(1);
    }
    Serial.println("CCS811 Init success");

    // init BME chute deploy variables  
    int counter = 0;
    float sum = 0;
    float start_time = millis();
    float last_alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
    pcounter = &counter;
    psum = &sum;
    pstart_time = &start_time;
    plast_alt = &last_alt;
    *plast_alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
}

void loop() {

    log_data();
    update_altitude(pcounter, psum, plast_alt, pstart_time);
    delay(20);

}

void log_data(){
  //TODO

  // order: 
  // altitude, pressure, temperature, humidity, x_a, y_a, z_a, x_g, y_g, z_g, 
  
}


void update_altitude(int *pcounter, float *psum, float *plast_alt, float *pstart_time) {
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    if(millis()-*pstart_time >CD_UPDATE_GAP){ //if it's been more than CD_UPDATE_GAP seconds
      // show the average value of the altitude over 1 sec
      pr("counter: ");
      prln(*pcounter);
      float avg = *psum/(*pcounter);
      pr("current altitude average: ");
      pr(avg);
      pr(" diff with last: ");
      float diff = avg-*plast_alt;
      prln(diff);
      // then, if there has been at least a meter of drop, deploy parachute
      if(diff < -1){
        deployChute();
      }
      //when all this is done, indicate what counter was at, update last_alt (=alt) reset counter, start_time, avg
      pr("previous counter number: ");
      pr(*pcounter);
      delay(2000);

      *plast_alt = avg;
      *pcounter = 1;
      *pstart_time = millis();
      *psum = 0;      
    }
    //update the altitude avg, then counter
    *psum += altitude;
    *pcounter += 1;   
}

void deployChute(){
  for(int i=0; i<20; i++){
      Serial.println("deploying ");
      digitalWrite(LEDPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(100);                       // wait for a second
      digitalWrite(LEDPIN, LOW);    // turn the LED off by making the voltage LOW
      delay(100); 
  }
}
