// class IMU handles the MPU6050
// Gunter Wiedemann
// 13.05.2023

// IMPORTANT: Wire.begin() must be started in main.cpp - not in the constructor of IMU

#include <Arduino.h>

#ifndef IMU_H
#define IMU_H


/**********************************/
/* IMU Address                    */
/**********************************/
#define MPU6050_ADDR            0x68 // Alternatively set AD0 to HIGH  --> Address = 0x69

/**********************************/
/* Registers                      */
/**********************************/

#define ACCXREG                 0x3B
#define ACCYREG                 0x3D
#define ACCZREG                 0x3F
#define TEMPREG                 0x41
#define GYROXREG                0x43
#define GYROYREG                0x45
#define GYROZREG                0x47
#define ACCSETREG               0x1C
#define LIM2G                   0x00
#define LIM4G                   0x08
#define LIM8G                   0x10
#define LIM16G                  0x18
#define GYROSETREG              0x1B
#define LIM250DPS               0x00
#define LIM500DPS               0x08
#define LIM1000DPS              0x10
#define LIM2000DPS              0x18
#define PWRREG                  0x6B
#define RESET_DEVICE            0x80




/**********************************/
/* class IMU                      */
/**********************************/

/// @brief contains all function and variables to process the IMU
class IMU
{

private:

    // current value
    int accX = 0;
    int accY = 0;
    int accZ = 0;
    int gyroX = 0;
    int gyroY = 0;
    int gyroZ = 0;
    int tRaw = 0;

    // offset handling
    byte accReg[3] = {ACCXREG, ACCYREG, ACCZREG};
    int accOffset[3] = {0, 0, 0}; // x, y, z
    byte gyroReg[3] = {GYROXREG, GYROYREG, GYROZREG};
    int gyroOffset[3] = {0, 0, 0}; // x, y, z 

    // internal states
    int maxG[4] = {2,4,8,16};
    byte maxGRegVal[4] = {LIM2G, LIM4G, LIM8G, LIM16G};
    
    int maxGState = 0;
    int maxDps[4] = {250, 500, 1000, 2000};
    byte maxDpsRegVal[4] = {LIM250DPS, LIM500DPS, LIM1000DPS, LIM2000DPS};
    int maxDpsState = 0;

    // Methods and vars for printing
    char result[7]; // temporary variable used in convert function
    char* toStr(int16_t character) { // converts int16 to string and formatting
        sprintf(result, "%6d", character);
        return result;
    }

public:

    IMU(/* args */);
    ~IMU();

    /// @brief resets the MPU6050
    void reset();

    /// @brief sets in register of the MPU6050
    /// @param reg register number to set
    /// @param val value to write
    void setReg(byte reg, byte val);

    /// @brief gets the value of a register of the MPU6050
    /// @param reg control error (how many mm/100 is the distance between set value and measuere value )
    /// @return register value
    byte getReg(byte reg);

    /// @brief gets the value of a 16bit register of the MPU6050
    /// @param reg control error (how many mm/100 is the distance between set value and measuere value )
    /// @return register value
    int getVal(byte reg);

    /// @brief gets all acceleration, temperature and gyro values
    void getAllVals();

    /// @brief prints all acceleration, temperature and gyro values
    /// @param errordigit control error (how many mm/100 is the distance between set value and measuere value )
    /// @param halldigit hall sensor readout
    void printVals();

    /// @brief sets the acceleration state of the MP6050:
    /// @param rangeState 
    ///         0 - +-2g (default)
    ///         1 - +-4g
    ///         2 - +-8g
    ///         3 - +-16g
    void setAccRange(int rangeState);

    /// @brief sets the acceleration state of the MP6050:
    /// @param rangeState 
    ///         0 - +-250dps (default)
    ///         1 - +-500dps
    ///         2 - +-1000dps
    ///         3 - +-2000dps
    void setGyroRange(int rangeState);

    /// @brief determines the offset of one acceleration axis :
    /// @param axis 
    ///         0 - x
    ///         1 - y
    ///         2 - z
    void calibrateACC(int axis, int value);

    /// @brief determines the offset of the gyro in all 3 axes
    void calibrateGyro();

    /// @brief determines the mean deviation of a value in 16bit register
    /// @param address address of the value the offset of which shall be determined 
    /// @param value value, the offset shall be determined to
    /// @returns mean value of the offset
    int calcOffset(byte address, int value);

};




#endif