/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-force-sensor
 */
#define FS0 A0 // the FSR and 10K pulldown are connected to A0
#define FS1 A1 // connection to A1
#define FS2 A2  //connection to A2
#include <SPI.h>
#include <SD.h>
File myfile;
const int ledPin = 8;

 

void setup() {
  pinMode(ledPin, OUTPUT);
  flash_warn();

  if(!SD.begin()){

    while(1);
  }
  myfile = SD.open("tele.txt", FILE_WRITE);
  if(!myfile){

    while(1);
  }
  flash_warn();
  myfile.println("------new log starting----------");
  int counter=0;
  while(counter < 10){
    float r0 = analogRead(FS0);
    float r1 = analogRead(FS1);
    float r2 = analogRead(FS2);
    
   
    double force_g = calculate_force(r0);

    myfile.print(r0);
    myfile.print(",");
    myfile.print(force_g);
    myfile.print(",");
    
  
  
    force_g = calculate_force(r1);

    myfile.print(r1);
    myfile.print(",");
    myfile.print(force_g);
    myfile.print(",");
  
 
    force_g = calculate_force(r2);

    myfile.print(r2);
    myfile.print(",");
    myfile.println(force_g);
  
  
    delay(1000);
    counter++;
  }
  myfile.close();

  flash_warn();
}

void flash_warn(){

  for(int i=0; i<10; i++){
    digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)

    delay(500);                       // wait for a second
    digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
    delay(500); 
   }
}

double calculate_force(float r){
  float exponent = ((1.489*r*5)/1024);
  float base = 2.718;
  return(6.7249*pow(base, exponent));
}

void loop() {
  

/*
  
  float force_n = force_g/1000*9.8;
  Serial.print(" ///approx. newton value = ");
  Serial.print(force_n);
  Serial.print("\n");

/*

  if (counter < 5)
  {
     avg_data += analogReading;
     pr("\n");
     pr(counter);
     counter = counter + 1;
     pr("\n\n yes here do stuff ");
     pr(counter);
     pr(" \n\n here done \n\n"); 
  }
  else
  {
    counter = 1;
    Serial.print("\n average = ");
    Serial.print(avg_data/5);
    avg_data = analogReading;    
  }
  /*

  if (analogReading < 10)       // from 0 to 9
    Serial.println(" -> no pressure");
  else if (analogReading < 200) // from 10 to 199
    Serial.println(" -> light touch");
  else if (analogReading < 500) // from 200 to 499
    Serial.println(" -> light squeeze");
  else if (analogReading < 800) // from 500 to 799
    Serial.println(" -> medium squeeze");
  else // from 800 to 1023
    Serial.println(" -> big squeeze");
    */


  }
