// all sensors together. all basic required funcitonnality there
// launch detection by timer LAUNCH_COUNTDOWN in MILLISECONDS (2 mins = 120000  ;   5 min = 300000   ;   10 min = 600000
// peak detection by pressure + acceleration (acceleration less than 11m/s² on the z axis)
// 95% of board memory with current sketch

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MPU6050.h>
#include <Servo.h>
#include <SD.h>

//---------need to include servo files----------------

#define SEALEVELPRESSURE_HPA (1013.25)
#define CD_UPDATE_GAP 3000 // how often to check for info (flight telemetry)
#define LAUNCH_COUNTDOWN 5000 // 5 SECONDS TO TEST-----------
#define SAFEGUARD_TIMER 15000 // after 15 seconds of flight, the file automatically closes, regardless of the status of the parachute

Adafruit_BME280 bme;
Adafruit_MPU6050 mpu;
Servo myservo;
File file;

int clock_init; // this is the base clock reference for the program

//setting up chute update
int counter;
int sum;
unsigned int start_time;
short int curr_alt;
short int last_alt;

// the accelerations to check if peak is actually reached
sensors_event_t a, g, t;
void setup() {
    // settup all sensors, modules, servo, files
    Serial.begin(9600);

    clock_init = millis();
    
    // start BME sensor
    bool rslt;
    rslt = bme.begin();  
    if (!rslt) {
        Serial.println("BME280 Init Fail,Please Check your address or the wire you connected!!!");
        while (1);
    }  
    Serial.println("BME280 Init Success");

    // start mpu6050 sensor
    if (!mpu.begin()) {
      Serial.println("Failed to find MPU6050 chip");
      while (1);
    }
    Serial.println("MPU6050 Init success!");

    // settup servo
    myservo.attach(8);
    myservo.write(0); // this is the only time the servo can be set at 0
  

    // init BME chute deploy variables  
    counter = 0;
    sum = 0;
    start_time = millis();
    last_alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
    
    // settup mpu range (accel & gyro)
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    pre_armed();
}

void pre_armed(){
  //TODO: WAIT LAUNCH COUNTDOWN TIMER, THEN ENTER FLY PHASE
  while(1){
    if(millis()-clock_init > LAUNCH_COUNTDOWN){
      // ENTER FLY PHASE
      flight();
      while(1); // can never have 2 flight phases
    }
  }
  
}

void flight(){
  //TODO : LOG DATA + CHECK FOR PEAK + CHECK FOR PAST SAFEGUARD TIMER
  // LOG DATA FIRST
  // THEN UPDATE ALTITUDE (CHECK PEAK FUNCTION)
  // THEN CHECK FOR PAST SAFEGUARD TIMER (or as part of update alt?)
  while(1){
    log_data();  
    //important data for update are already measured, they must just update other values
    update_altitude();      
  }
  //(UPDATE ALTITUDE OR SAFEGUARD) TIMER WILL DEPLOY CHUTE
}

void log_data(){
  //TODO
  
  // order: 
  // altitude, pressure, temperature, humidity, x_a, y_a, z_a, x_g, y_g, z_g 

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
  curr_alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.println(curr_alt);
  Serial.println("m--------------------------------BME280------------------------------------");

  // mpu test code
    /* Get new sensor events with the readings */

  mpu.getEvent(&a, &g, &t);
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

  Serial.println("---------------------------------MPU6050--------------------------------------");


  //log to file
  file.print(bme.readTemperature());
  file.print(",");
  file.print(bme.readPressure()/100.0F);
  file.print(",");
  file.print(bme.readHumidity());
  file.print(",");
  curr_alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  file.println(curr_alt);

    /* Print out the values */
  file.print(",");
  file.print(a.acceleration.x);
  file.print(",");
  file.print(a.acceleration.y);
  file.print(",");
  file.print(a.acceleration.z);
  file.print(",");

  file.print("Rotation X: ");
  file.print(g.gyro.x);
  file.print(",");
  file.print(g.gyro.y);
  file.print(",");
  file.print(g.gyro.z);
  
}

void update_altitude() {
    // altitude stored in curr_alt
    if(millis()-start_time >CD_UPDATE_GAP){ //if it's been more than CD_UPDATE_GAP seconds
      // show the average value of the altitude over 1 sec
      Serial.print("counter: ");
      Serial.println(counter);
      float avg = sum/(counter);
      Serial.print("current altitude average: ");
      Serial.println(avg);
      Serial.print(" diff with last: ");
      float diff = avg-last_alt;
      Serial.println(diff);
      // then, if there has been at least a meter of drop, deploy parachute
      if(diff < -1){
        deployChute();
      }      
      //when all this is done, indicate what counter was at, update last_alt (=alt) reset counter, start_time, avg

      last_alt = avg;
      counter = 1;
      start_time = millis();
      sum = 0;      
    }

    //check if past the safeguard timer
    if(millis()-(clock_init+LAUNCH_COUNTDOWN)>SAFEGUARD_TIMER){
      deployChute();
    }
    
    //update the altitude avg, then counter
    sum += curr_alt;
    counter += 1;   
}

void deployChute() {
   //TODO: 
   // turn knob 90 degrees
   myservo.write(90);
   Serial.println("servo spun");
   // close file safely
   file.close();
   Serial.println("file closed");
   while(1){
    Serial.println("awaiting dispatch");
    delay(5000);
   }
   //
}
