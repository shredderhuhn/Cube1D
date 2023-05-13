#include <Arduino.h>

// using a 200-step motor (most common)
#define MOTOR_STEPS 200
// configure the pins connected
#define DIR 8
#define STEP 9
#define MS1 10
#define MS2 11
#define MS3 12


#include "Wire.h" 
 
#define MPU6050_ADDR 0x68 // Alternatively set AD0 to HIGH  --> Address = 0x69
int16_t accX, accY, accZ, gyroX, gyroY, gyroZ, tRaw; // Raw register values (accelaration, gyroscope, temperature)
char result[7]; // temporary variable used in convert function

char* toStr(int16_t character) { // converts int16 to string and formatting
  sprintf(result, "%6d", character);
  return result;
}

int waittime = 1000;
unsigned long  time1, time2, time3, deltatime1, deltatime2;

void setup() {
  //pinMode(STEP,OUTPUT);
  //pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // wake up!
  Wire.endTransmission(true);
  
}


void loop() {

  time1 = micros();
  
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. 
                               // As a result, the connection is kept active.
  Wire.requestFrom(MPU6050_ADDR, 14, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same int16_t variable
  accX = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accY = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accZ = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  tRaw = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyroX = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyroY = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyroZ = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  


  time2 = micros();
  
  Serial.print("AcX = "); Serial.print(toStr(accX));
  Serial.print(" | AcY = "); Serial.print(toStr(accY));
  Serial.print(" | AcZ = "); Serial.print(toStr(accZ));
  // from data sheet:
  Serial.print(" | tmp = "); Serial.print((tRaw + 12412.0) / 340.0);
  Serial.print(" | GyX = "); Serial.print(toStr(gyroX));
  Serial.print(" | GyY = "); Serial.print(toStr(gyroY));
  Serial.print(" | GyZ = "); Serial.print(toStr(gyroZ));
  Serial.println();

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