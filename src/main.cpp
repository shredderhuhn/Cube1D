#include <Arduino.h>
#include <IMU.h>
#include "Wire.h" 

// using a 200-step motor (most common)
#define MOTOR_STEPS 200
// configure the pins connected
#define DIR 8
#define STEP 9
#define MS1 10
#define MS2 11
#define MS3 12


IMU imu;

int waittime = 1000;
unsigned long  time1, time2, time3, deltatime1, deltatime2;


void setup() {
  //pinMode(STEP,OUTPUT);
  //pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);

  Wire.begin();
  imu.reset();
  
}


void loop() {

  time1 = micros();  
  imu.getAllVals();
  time2 = micros(); 
  imu.printVals();
  time3 = micros();

  deltatime1 = time2 - time1;
  deltatime2 = time3 - time2;

  Serial.print("I2C-Auslesezeit: "); Serial.print(deltatime1); Serial.println(" us");
  Serial.print("I2C-Printzeit: "); Serial.print(deltatime2); Serial.println(" us");
  
  delay(1000);
  
  //digitalWrite(STEP, HIGH);
  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(waittime);

  //digitalWrite(STEP,LOW);
  //digitalWrite(LED_BUILTIN,LOW);
  //delay(waittime);

  //waittime ++;
}
