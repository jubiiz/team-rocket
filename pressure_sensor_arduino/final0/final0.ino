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
File file;
void setup() {
  Serial.begin(9600);
  while(!Serial){
    ;
  }

  if(!SD.begin(10)){
    Serial.println("sd failure ");
    while(1);
  }
  file = SD.open("telemetry.txt", FILE_WRITE);
  if(file){
    Serial.println("success file");
  }
  else{
    Serial.println("file failure");
    while(1);
  }
  int counter = 0;
  while(counter<10){
    float r0 = analogRead(FS0);
    float r1 = analogRead(FS1);
    float r2 = analogRead(FS2);
    
    Serial.print("Sensor 1 = ");
    Serial.print(r0); // print the raw analog reading      
    double force_g = calculate_force(r0);
    Serial.print("   Approx. gram value = ");
    Serial.print(force_g);
    file.print(r0);
    file.print(",");
    file.print(force_g);
    file.print(",");
  
    Serial.print("Sensor 2 = ");
    Serial.print(r1); // print the raw analog reading     
    force_g = calculate_force(r1);
    Serial.print("   Approx. gram value = ");
    Serial.print(force_g);
     file.print(r1);
    file.print(",");
    file.print(force_g);
    file.print(",");
  
    Serial.print("Sensor 2 = ");
    Serial.print(r2); // print the raw analog reading      
    force_g = calculate_force(r2);
    Serial.print("   Approx. gram value = ");
    Serial.println(force_g);
    file.print(r2);
    file.print(",");
    file.print(force_g);
    file.print(",");
  
    delay(1000);
    
    counter++;
  }

  file.close();
  Serial.println("closed");
  
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
