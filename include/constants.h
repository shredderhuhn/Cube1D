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
#define ENA                         3   // LOW = enabled

/**********************************/
/* Motor constants                */
/**********************************/
// are in motor.h

/*************************************/
/* Parameter of the MPU6050 Sensor   */
/*************************************/


/************************************/
/* Parameter of the Control Loop    */
/************************************/

#define INITIALSETPOINT             0           // [1/16384*sin(alpha_soll)/pi/2]
#define MINSETPOINT                 -1000       // [1/16384*sin(alpha_soll)/pi/2]
#define MAXSETPOINT                 +1000       // [1/16384*sin(alpha_soll)/pi/2]
//#define INITIALCURRENTOFFSET        1000        // vorher: 780         // [digit]
#define MAXABSMOTORACC              25768       // [digit] bzw [10urad/s^2] // maxmimaler Betrag: 257.68rad/s^2
#define MAXABSMOTORVEL              6283185     // [digit] bzw [10urad/s]   // 500us ticks -> 2000ticks/s = 10U/s = 2pi*10rad/s
#define SAMPLETIME                  1000        // [µs]      // 1kHz Takt für den Anfang  
#define K0Z                         18750      
#define K1Z                         -179
#define K2Z                         236
#define K3Z                         10
#define TAZZ                        (SAMPLETIME  << 10) / 1000 // ist 1*2^10 bei 1ms Abtastzeit
#define KSTEPZZ                     392699082     // gueltig bei 200ticks/U = pi * 10^9 * 2^-3

#endif