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
#define pr(msg);  Serial.print(msg)
void setup() {
  Serial.begin(9600);
}

void loop() {
  float r0 = analogRead(FS0);
  float r1 = analogRead(FS1);

  Serial.print("Force sensor reading = ");
  Serial.print(r0); // print the raw analog reading
  Serial.print(" voltage out: ");
  Serial.print((r0*5)/1000);
  float exponent = ((1.489*r0*5)/1000);
  float base = 2.718;
  
    
  double force_g = 6.7249*pow(base, exponent);
  Serial.print(" ///Approx. gram value = ");
  Serial.print(force_g);

  pr(" /// //// /// sensor 2: ")
  pr(" reading 2: ")
  pr(r1);
  pr("\n");
  Serial.println();

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

  delay(1000);
}