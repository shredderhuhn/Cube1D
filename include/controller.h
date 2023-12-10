// All Controller variables and functions
// no class to avoid overhead
// Gunter Wiedemann
// 03.12.2023

#include <constants.h>
#include <Arduino.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

/***************************************/
/* Structures that aren't constants    */
/***************************************/

/// @brief contains all variables that can be changed by interaction
/// @param state State of the state machine - int 0...10
/// @param setpoint Setpoint of the control loop - int 1000 ... 2000 [1/100 mm]
/// @param offset Current offset of the control output -  0 ... 4095 [digit]
/// @param failure failure variable for wrong/lost ball position
struct Status
{
    int state;
    int setpoint;
    int offset;
    bool failure;
};

struct Ctrl
{
    int setpoint;
    int offset;
    int error;
    int errorTildeK;
    int errorTildeKM1;
    int u1K;
    int u2K;
    int u2KM1;
    int u;
    int v;  //Geschwindigkeit
    int tick;
    bool dir;
};


extern Status status;
extern Ctrl ctrl;


// Functions

/// @brief PD-controller, returning the angular acceleration of the whell in 10mrad/s^2
/// @param measpoint measured x-value of the MPU6050
/// @return nothing, the return value ctrl.u is a global variable
void calcController(int measpoint);

void resetController(void);

void initController(void);

/// @brief calculating from ctrl.u [10mrad/s^2] into tick in [us]
void calcTick(void);

void setOutputValues(void);

void resetOutputValues(void);

void setCurrent(void);

void resetCurrent(void);

void resetState(Status &status);

#endif