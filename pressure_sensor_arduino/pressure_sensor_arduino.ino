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

  int counter = 0;
  float avg_data = 0;
  int *pcounter = &counter;
  float *pavg_data = &avg_data;
  
  while(true){
    update_pressure(pcounter, pavg_data);
    delay(1000);
  }
}

void update_pressure(int *pcounter, float *pavg_data){
  int counter = *pcounter;
  float avg_data = *pavg_data;

  float e = 2.693;
  float analogReading = analogRead(FORCE_SENSOR_PIN);

  Serial.print("Force sensor reading = ");
  Serial.print(analogReading); // print the raw analog reading

  float force_g = 30.096*(pow(e, (0.0032
  *analogReading)));
  Serial.print(" ///Approx. gram value = ");
  Serial.print(force_g);

  
  float force_n = force_g/1000*9.8;
  Serial.print(" ///approx. newton value = ");
  Serial.print(force_n);
  Serial.print("\n");
  
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
  *pcounter = counter;
  *pavg_data = avg_data;
  
  
}

void loop() {
}
