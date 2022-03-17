// all sensors together. all basic required funcitonnality there
// launch detection by timer LAUNCH_COUNTDOWN in MILLISECONDS (2 mins = 120000  ;   5 min = 300000   ;   10 min = 600000
// peak detection by pressure + acceleration (acceleration less than 11m/s² on the z axis)

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_MPU6050.h>
#include <Servo.h>
#include <SD.h>

//---------need to include servo files----------------

#define SEALEVELPRESSURE_HPA (1013.25)
#define CD_UPDATE_GAP 3000 // how often to check for info (flight telemetry)
#define LAUNCH_COUNTDOWN 5000 // 5 SECONDS TO TEST-----------
#define SAFEGUARD_TIMER 15000 // after 15 seconds, the file automatically closes, regardless of the status of the parachute

Adafruit_BME280 bme;
Adafruit_CCS811 ccs;
Adafruit_MPU6050 mpu;
Servo myservo;
File file;

int clock_init; // this is the base clock reference for the program

//setting up chute update
int *pcounter;
float *psum;
float *pstart_time;
float *plast_alt;

// the z acceleration to check if peak is actually reached
float z_accel;
void setup() {
    // settup all sensors, modules, servo, files
    Serial.begin(9600);


    Serial.print("this is a test message to see how many bytes it takes to write such a huge message");

    clock_init = millis();
    
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

    // start mpu6050 sensor
    if (!mpu.begin()) {
      Serial.println("Failed to find MPU6050 chip");
      while (1);
    }
    Serial.println("MPU6050 Init success!");

    // settup servo
    myservo.attach(8);
    myservo.write(0);
  

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

    // settup mpu range (accel & gyro)
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    pre_armed();
}

void pre_armed(){
  //TODO: WAIT LAUNCH COUNTDOWN TIMER, THEN ENTER FLY PHASE
  
  
}

void flight(){
  //TODO : LOG DATA + CHECK FOR PEAK + CHECK FOR PAST SAFEGUARD TIMER
  // LOG DATA FIRST
  // THEN UPDATE ALTITUDE (CHECK PEAK FUNCTION)
  // THEN CHECK FOR PAST SAFEGUARD TIMER

  //UPDATE ALTITUDE OR SAFEGUARD TIMER WILL DEPLOY CHUTE
}

void loop() {

    log_data();
    //update_altitude(pcounter, psum, plast_alt, pstart_time);
    delay(20);

}

void log_data(){
  //TODO
  
  // order: 
  // altitude, pressure, temperature, humidity, x_a, y_a, z_a, x_g, y_g, z_g, CO2, TVOC 

  // BME280 test code
  Serial.print("temperature:");
  Serial.print(bme.readTemperature());
  Serial.print("*C   ");

  Serial.print("pressure:");
  Serial.print(bme.readPressure()/100.0F);
  Serial.print("hPa   ");

  Serial.print("humidity:");
  Serial.print(bme.readHumidity());
  Serial.print("%   ");

  Serial.print("altitude:");
  Serial.println(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println("m--------------------------------BME280------------------------------------");

  // mpu test code
    /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("---------------------------------MPU6050--------------------------------------");

  if(ccs.available()){
    if(!ccs.readData()){
      Serial.print(" CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.println(ccs.getTVOC());
      Serial.println("----------------------------------------CCS811--------------------------------");
      }   

  
}
  delay(2000);
}


void update_altitude(int *pcounter, float *psum, float *plast_alt, float *pstart_time) {
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    if(millis()-*pstart_time >CD_UPDATE_GAP){ //if it's been more than CD_UPDATE_GAP seconds
      // show the average value of the altitude over 1 sec
      Serial.print("counter: ");
      Serial.println(*pcounter);
      float avg = *psum/(*pcounter);
      Serial.print("current altitude average: ");
      Serial.print(avg);
      Serial.print(" diff with last: ");
      float diff = avg-*plast_alt;
      Serial.println(diff);
      // then, if there has been at least a meter of drop, deploy parachute
      if(diff < -1){
        deployChute();
      }
      //when all this is done, indicate what counter was at, update last_alt (=alt) reset counter, start_time, avg
      Serial.print("previous counter number: ");
      Serial.print(*pcounter);
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

void deployChute() {
   //TODO: 
   // turn knob 90 degrees
   myservo.write(90);
   Serial.println("servo spun----------");
   // close file safely
   file.close();
   Serial.println("file closed-----------");
   while(1){
    Serial.println("awaiting dispatch");
    delay(5000);
   }
   //
}
