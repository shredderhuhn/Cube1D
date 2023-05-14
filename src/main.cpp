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

// Methods and vars for printing
char result[7]; // temporary variable used in convert function
char* toStr(int16_t character) { // converts int16 to string and formatting
    sprintf(result, "%6d", character);
    return result;
}

IMU imu;

int waittime = 1000;
int16_t temp = -1;
unsigned long  time1, time2, time3, deltatime1, deltatime2;


void setup() {
  //pinMode(STEP,OUTPUT);
  //pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);

  Wire.begin();
  imu.reset();
  //imu.calibrateGyro();
  //imu.calibrateACC(0,-16000);
  //imu.calibrateACC(1,0);
  //imu.calibrateACC(2,0);
  
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

  temp = imu.getVal(GYROXREG);
  Serial.print("GyX = "); Serial.println(toStr(temp));
  Serial.print("GyX ohne Str = "); Serial.println(temp);

  //imu.calibrateGyro();

  delay(10000);
  
  //digitalWrite(STEP, HIGH);
  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(waittime);

  //digitalWrite(STEP,LOW);
  //digitalWrite(LED_BUILTIN,LOW);
  //delay(waittime);

  //waittime ++;
}
