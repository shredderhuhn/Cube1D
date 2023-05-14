// global constants of the Cube1D project
// Gunter Wiedemann
// 13.05.2023

#ifndef CONSTANTS_H
#define CONSTANTS_H


/**********************************/
/* General Constants              */
/**********************************/

#define ON                          1
#define OFF                         0
#define VALID                       1
#define INVALID                     0


/**********************************/
/* Arduino Due Port Definition    */
/**********************************/

// IMU-Wire-IF uses SCL/SDA-pins (21,20)

// Motor driver 8825
#define DIR                         9
#define STEP                        8
#define RST                         7   // needs to be HIGH to work
#define MS2                         6
#define MS1                         5
#define MS0                         4
#define ENABLE                      3   // LOW = enabled



/*************************************/
/* Parameter of the MPU6050 Sensor   */
/*************************************/


/************************************/
/* Parameter of the Control Loop    */
/************************************/



#endif