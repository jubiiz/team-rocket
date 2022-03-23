#include <Wire.h>
#include <SPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SD.h>
File file;

//for faster prints
#define pr(msg) Serial.print(msg)
#define prln(msg) Serial.println(msg)
#define USE_SD true

//if you need to read altitude,you need to know the sea level pressure
#define SEALEVELPRESSURE_HPA (1013.25)

//This Macro definition decide whether you use I2C or SPI
//When USEIIC is 1 means use I2C interface, When it is 0,use SPI interface
#define USEIIC 1
#define LEDPIN 2


/*
This Demo is tested on UNO PLUS
SPI:
SPI_SCK:  D13
SPI_MISO: D12
SPI_MOSI: D11
SPI_CS:   D10

I2C:
I2C_SCL:  A5
I2C_SDA:  A4
the default I2C address is 0x77, you can change it in Adafruit_BME280.h
*/

#if(USEIIC)
  Adafruit_BME280 bme;
#else
  #define SPI_SCK 13
  #define SPI_MISO 12
  #define SPI_MOSI 11
  #define SPI_CS 10
  Adafruit_BME280 bme(SPI_CS, SPI_MOSI, SPI_MISO, SPI_SCK);
#endif

unsigned long delayTime;
int counter;
float sum;
float start_time;
float last_alt;


void setup() {

    Serial.begin(9600);
    Serial.println("program initiating");
    pinMode(LEDPIN, OUTPUT); 
    bool rslt;
    rslt = bme.begin();  
    if (!rslt) {
        Serial.println("Init Fail,Please Check your address or the wire you connected!!!");
        while (1);
    }
  
    Serial.println("bme Init Success");
    if(USE_SD){
      if (!SD.begin(10)){
        Serial.println("sd failure");
        while(1);
      }
      file = SD.open("pre.txt", FILE_WRITE);
      if(file){
        Serial.println("success file");
        digitalWrite(LEDPIN, HIGH);
        delay(2000);
        file.println("------------NEW READING------------");
      }
      else{
        Serial.print("file failure");
        while(1);
      }
    }
    deployChute(false);

    counter = 0;
    sum = 0;
    start_time = millis();
    last_alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

    //update_altitude(&counter, &sum, &last_alt, &start_time);
}

void loop() { 
    Serial.print("  loop counter: ");
    Serial.println(counter);
    update_altitude();
    delay(20);
}

void update_altitude() {
    Serial.println("updating altitude");
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    Serial.print("altitude: ");
    Serial.println(altitude);
    if(millis()-start_time >3000){ //if it's been more than 3 seconds
      // show the average value of the altitude over 1 sec
      pr("counter: ");
      prln(counter);
      counter += 1;
      Serial.println(counter);
      float avg = sum/(counter);
      pr("current altitude average: ");
      pr(avg);
      pr(" diff with last: ");
      float diff = avg-last_alt;
      prln(diff);

      if(USE_SD){
        //log to sd
        file.print("current: ");
        file.print(avg);
        file.print(", diff");
        file.println(diff);
      }

      
      // then, if there has been at least a meter of drop, deploy parachute
      if(diff < -0.5){
        deployChute(true);
      }

      //when all this is done, indicate what counter was at, update last_alt (=alt) reset counter, start_time, avg
      pr("previous counter number: ");
      pr(counter);
      delay(2000);

      last_alt = avg;
      counter = 1;
      start_time = millis();
      sum = 0;
      
    }

    //update the altitude avg, then counter
    sum += altitude;
    counter += 1;

/*
    pr(" sum/counter, last avg, sum, counter");
    pr(*sum/(*counter));
    pr(" ");
    pr(*last_alt);
    pr(" ");
    pr(*sum);
    pr(" ");
    prln(*counter);
    */   
}

void deployChute(bool c){

  if(USE_SD){
    file.println("############CHUTE DEPLOY###############");
     if(c){
      file.close();
      Serial.print("closed");
      digitalWrite(LEDPIN, HIGH);
      while(1);
    }
  }
  for(int i=0; i<20; i++){
      Serial.println("deploying ");
      digitalWrite(LEDPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(100);                       // wait for a second
      digitalWrite(LEDPIN, LOW);    // turn the LED off by making the voltage LOW
      delay(100); 
  }
}
