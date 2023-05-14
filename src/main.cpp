#include <Arduino.h>
#include <IMU.h>
#include "Wire.h" 

// using a 200-step motor (most common)
#define MOTOR_STEPS 200
// configure the pins connected
#define DIR                         9
#define STEP                        8
#define RST                         7   // needs to be HIGH to work
#define MS2                         6
#define MS1                         5
#define MS0                         4
#define ENA                         3   // HIGH = stop

// Methods and vars for printing
char result[7]; // temporary variable used in convert function
char* toStr(int16_t character) { // converts int16 to string and formatting
    sprintf(result, "%6d", character);
    return result;
}

IMU imu;

int waittime = 1000;
int16_t temp = -1;
int testsum = 0;
unsigned long  time1, time2, time3, deltatime1, deltatime2;


void setup() {
  pinMode(ENA,OUTPUT);
  pinMode(STEP,OUTPUT);
  pinMode(DIR,OUTPUT);
  pinMode(RST,OUTPUT);
  pinMode(MS0, OUTPUT);
  pinMode(MS1,OUTPUT);
  pinMode(MS2,OUTPUT);
  digitalWrite(ENA,LOW);
  digitalWrite(DIR,LOW);
  digitalWrite(MS0,LOW);
  digitalWrite(MS1,LOW);
  digitalWrite(MS2,LOW);
  digitalWrite(RST,HIGH);

  //pinMode(LED_BUILTIN,OUTPUT);

  /*
  Serial.begin(9600);

  Wire.begin();
  imu.reset();
  imu.calibrateGyro();
  imu.calibrateACC(0,-16384);
  imu.calibrateACC(1,0);
  imu.calibrateACC(2,0);
  imu.printOffsets();
  */
}


void loop() {
  /*
  time1 = micros();  
  imu.getAllVals();
  time2 = micros(); 
  imu.printVals();
  //imu.printRawVals();
  time3 = micros();

  deltatime1 = time2 - time1;
  deltatime2 = time3 - time2;

  Serial.print("I2C-Auslesezeit: "); Serial.print(deltatime1); Serial.println(" us");
  Serial.print("I2C-Printzeit: "); Serial.print(deltatime2); Serial.println(" us");

  delay(10000);

  */
  
  digitalWrite(STEP, HIGH);
  //digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(waittime);

  digitalWrite(STEP,LOW);
  //digitalWrite(LED_BUILTIN,LOW);
  delayMicroseconds(waittime);

  //waittime ++;
}
