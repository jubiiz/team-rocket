 /*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-force-sensor
 */
const byte FS[] = {A0, A1, A2}; // the FSR and 10K pulldown are connected to A0;
const int NUM_SENSORS = 3;
const int FLIGHT_TIME = 7; // in seconds
const int DELAY_TIME = 10;

#define ledPin 3
#include <SPI.h>
#include <SD.h>
File file;
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  if(!SD.begin(10)){
    Serial.println("sd failure ");
    while(1);
  }
  file = SD.open("tele.txt", FILE_WRITE);
  if(file){
    Serial.println("success file");
  }
  else{
    Serial.println("file failure");
    while(1);
  }

  // warn of imminent liftoff
  flash_warn();
  
  int counter = 0;
  float r;
  double w;
  file.println("------------NEW LOG-----------");
  while(counter<((FLIGHT_TIME*1000)/DELAY_TIME)){

    for(int i=0; i<NUM_SENSORS; i++){
      //get data
      r = analogRead(FS[i]);
      //transform to weight
      w = calculate_force(r);
      //output data,weight,
      file.print(r);
      file.print(",");
      file.print(w);
      file.print(",");

      Serial.print(r);
      Serial.print(",");
      Serial.print(w);
      Serial.print(",");
    }
    file.print("\n");
    Serial.print("\n");
  
    delay(DELAY_TIME);    
    counter++;
  }

  file.close();
  Serial.println("closed");
  flash_warn();
  
}

void flash_warn(){
  for(int t=0; t<10; t++){
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }  
}

double calculate_force(float r){
  float exponent = ((1.489*r*5)/1024);
  float base = 2.718;
  return(6.7249*pow(base, exponent));
}

void loop() {
// nothing to see here
  }
