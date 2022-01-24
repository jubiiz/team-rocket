/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-force-sensor
 */
#define FORCE_SENSOR_PIN A0 // the FSR and 10K pulldown are connected to A0
#define pr(msg) Serial.print(msg)
void setup() {
  Serial.begin(9600);
}

void loop() {
  float analogReading = analogRead(FORCE_SENSOR_PIN);

  Serial.print("Force sensor reading = ");
  Serial.print(analogReading); // print the raw analog reading
  Serial.print(" voltage out: ");
  Serial.print((analogReading*5)/1000);
  float exponent = ((1.489*analogReading*5)/1000);
  float base = 2.718;
  
    
  double force_g = 6.7249*pow(base, exponent);
  Serial.print(" ///Approx. gram value = ");
  Serial.print(force_g);
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
