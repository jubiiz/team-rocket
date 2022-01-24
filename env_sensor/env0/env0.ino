#include <Wire.h>
#include <SPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//for faster prints
#define pr(msg) Serial.print(msg)
#define prln(msg) Serial.println(msg)

//if you need to read altitude,you need to know the sea level pressure
#define SEALEVELPRESSURE_HPA (1013.25)

//This Macro definition decide whether you use I2C or SPI
//When USEIIC is 1 means use I2C interface, When it is 0,use SPI interface
#define USEIIC 0

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
int *pcounter;
float *psum;
float *pstart_time;
float *plast_alt;
void setup() {
    Serial.begin(115200);
  
    bool rslt;
    rslt = bme.begin();  
    if (!rslt) {
        Serial.println("Init Fail,Please Check your address or the wire you connected!!!");
        while (1);
    }
  
    Serial.println("Init Success");

    int counter = 0;
    float sum = 0;
    float start_time = millis();
    float last_alt = bme.readAltitude(SEALEVELPRESSURE_HPA);

    pcounter = &counter;
    psum = &sum;
    pstart_time = &start_time;
    plast_alt = &last_alt;
    *plast_alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
    
    //update_altitude(&counter, &sum, &last_alt, &start_time);
}

void loop() { 
    update_altitude(pcounter, psum, plast_alt, pstart_time);
    delay(20);
}

void update_altitude(int *pcounter, float *psum, float *plast_alt, float *pstart_time) {
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    if(millis()-*pstart_time >3000){
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
        for(int i=0; i<100; i++){
          prln("***************DEPLOY PARACHUTE*********************");
        }
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
