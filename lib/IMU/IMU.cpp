// class IMU handles the MPU6050
// Gunter Wiedemann
// 13.05.2023

#include <Arduino.h>
#include <IMU.h>
//#include <constants.h>
#include <Wire.h>

IMU::IMU()
{ 
}

IMU::~IMU()
{
}

void IMU::reset() {
    //setReg(PWRREG, RESET_DEVICE); // Unsinn?
    //delay(20);
    setReg(PWRREG,0);
    delay(20);
}

void IMU::setReg(byte reg, byte val) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg); // PWR_MGMT_1 register
    Wire.write(val); // Reset
    Wire.endTransmission(true);
}

byte IMU::getReg(byte reg) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false); // dont stop
    Wire.requestFrom(MPU6050_ADDR, 1, true); // request 1 register
    return Wire.read();
}

int IMU::getVal(byte reg) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false); // dont stop
    Wire.requestFrom(MPU6050_ADDR, 2, true); // request 2 registers
    int out =  Wire.read()<<8 | Wire.read();
    return out;
}

void IMU::getAllVals() {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(ACCXREG); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. 
                               // As a result, the connection is kept active.
    Wire.requestFrom(MPU6050_ADDR, 14, true); // request a total of 7*2=14 registers
  
    // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same int16_t variable
    accX = (Wire.read()<<8 | Wire.read()) - accOffset[0]; // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    accY = (Wire.read()<<8 | Wire.read()) - accOffset[1]; // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    accZ = (Wire.read()<<8 | Wire.read()) - accOffset[2]; // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    tRaw = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    gyroX = (Wire.read()<<8 | Wire.read()) - gyroOffset[0]; // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    gyroY = (Wire.read()<<8 | Wire.read()) - gyroOffset[1]; // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    gyroZ = (Wire.read()<<8 | Wire.read()) - gyroOffset[2]; // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
}

void IMU::printVals() {
    Serial.print("AcX = "); Serial.print(toStr(accX));
    Serial.print(" | AcY = "); Serial.print(toStr(accY));
    Serial.print(" | AcZ = "); Serial.print(toStr(accZ));
    Serial.print(" | tmp = "); Serial.print((tRaw + 12412.0) / 340.0);
    Serial.print(" | GyX = "); Serial.print(toStr(gyroX));
    Serial.print(" | GyY = "); Serial.print(toStr(gyroY));
    Serial.print(" | GyZ = "); Serial.println(toStr(gyroZ));
    Serial.print("AOX ");Serial.print(accOffset[0]);
    Serial.print("   AOY ");Serial.print(accOffset[1]);
    Serial.print("   AOZ ");Serial.println(accOffset[2]);
    Serial.print("GOX ");Serial.print(gyroOffset[0]);
    Serial.print("   GOY ");Serial.print(gyroOffset[1]);
    Serial.print("   GOZ ");Serial.println(gyroOffset[2]);
}

void IMU::setAccRange(int rangeState) {
    if (rangeState == 1 || rangeState ==2 || rangeState == 3) {
        setReg(ACCSETREG, maxGRegVal[rangeState]);
    } else {
        // default state = 0
        setReg(ACCSETREG,maxGRegVal[0]);
    }
        
}

void IMU::setGyroRange(int rangeState) {
    if (rangeState == 1 || rangeState ==2 || rangeState == 3) {
        setReg(GYROSETREG, maxDpsRegVal[rangeState]);
    } else {
        // default state = 0
        setReg(GYROSETREG,maxDpsRegVal[0]);
    }
        
}

void IMU::calibrateACC(int axis, int value) {
    if ((axis==0)||(axis==1)||(axis==2)) {
        byte address = ACCXREG + (0x02 * axis);
        accOffset[axis] = calcOffset(address, value);
    }
    return;
}

void IMU::calibrateGyro() {
    
    for  (int i = 0; i < 1; i++) {
        byte address = GYROXREG + (0x02 * i);        
        int temp = getVal(address);
        Serial.print(" | Gyro in Funktion = "); Serial.print(toStr(temp));
        delay(20);
        //gyroOffset[i] = calcOffset(address,0);
        int mean = getVal(address);
        //Serial.print("  mean = "); Serial.println((int)mean);
        Serial.print("  mean = "); Serial.println(toStr(mean));
        //int temp2 = calcOffset(address,0);
    }
    
   //int temp = getVal(GYROXREG);
   //Serial.print(" | GyX in Funktion = "); Serial.print(toStr(temp));
}

int IMU::calcOffset(byte address, int value) {
    /*
    int n = 1;
    int mean = 0;
    for (int i = 0; i < n; i++) {
        mean = getVal(address);
        //delay(20);
    }
    mean = mean/n;
    */
    int mean = getVal(address);
    Serial.print("  mean = "); Serial.println((int)mean);
    if ((mean > 32760) || (mean < -32760)) {
        mean = 0;
        Serial.println("mean rueckgesetzt auf 0.");
    }
    return (int)mean;
}