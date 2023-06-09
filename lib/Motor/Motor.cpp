#include <Motor.h>
#include <Arduino.h>

void Motor::init(uint8_t _dirPin, 
             uint8_t _stepPin, 
             uint8_t _rstPin, 
             uint8_t _ms2Pin, 
             uint8_t _ms1Pin, 
             uint8_t _ms0Pin, 
             uint8_t _enablePin) 
{
    dirPin = _dirPin;
    stepPin = _stepPin;
    rstPin = _rstPin;
    ms2Pin = _ms2Pin;
    ms1Pin = _ms1Pin;
    ms0Pin = _ms0Pin;
    enablePin = _enablePin;
    pinMode(dirPin,OUTPUT);
    pinMode(stepPin,OUTPUT);
    pinMode(rstPin,OUTPUT);
    pinMode(ms2Pin,OUTPUT);
    pinMode(ms1Pin, OUTPUT);
    pinMode(ms0Pin,OUTPUT);
    pinMode(enablePin,OUTPUT);

    disable();
    setMicroStepping(1);
    reset();
    //setDirection(dir);
}


bool Motor::setMicroStepping(uint8_t divider) {
    switch (divider)
    {
        case 1: //0 0 0
            digitalWrite(ms2Pin,LOW);
            digitalWrite(ms1Pin,LOW);
            digitalWrite(ms0Pin,LOW);
            ms = 1;
            return true;
        case 2: //0 0 1
            digitalWrite(ms2Pin,LOW);
            digitalWrite(ms1Pin,LOW);
            digitalWrite(ms0Pin,HIGH);
            ms = 2;
            return true;
        case 4: //0 1 0
            digitalWrite(ms2Pin,LOW);
            digitalWrite(ms1Pin,HIGH);
            digitalWrite(ms0Pin,LOW);
            ms = 4;
            return true;  
        case 8: //0 1 1
            digitalWrite(ms2Pin,LOW);
            digitalWrite(ms1Pin,HIGH);
            digitalWrite(ms0Pin,HIGH);
            ms = 8;
            return true;
        case 16: //1 1 1
            digitalWrite(ms2Pin,HIGH);
            digitalWrite(ms1Pin,HIGH);
            digitalWrite(ms0Pin,HIGH);
            ms = 16;
            return true;   
        default: //0 0 0
            digitalWrite(ms2Pin,LOW);
            digitalWrite(ms1Pin,LOW);
            digitalWrite(ms0Pin,LOW);
            ms = 1;
            return false;    
    }
        
}


void Motor::setDirection(uint8_t directionFlag) {
    if (directionFlag == 1) {
        digitalWrite(dirPin,HIGH);
        dir = 1;
    } else {
        digitalWrite(dirPin,0);
        dir = -1;
    }
}


void Motor::setVelocity(int16_t _velo) {
    if ((_velo<maxVelo) && (_velo>-maxVelo)) {
        velo = _velo;
    }
}

void Motor::setAcceleration(int16_t _acc) {
    if ((_acc<maxAcc) && (_acc>-maxAcc)) {
        acc = _acc;
    }
}


void Motor::setDeltaT(uint16_t time) {
    deltaT = time;
}


void Motor::setStepTick(uint16_t tick){
    ///TODO: Tick berechnen, in Abhängigkeit von acc, mit max/min velo
    ///TODO: v_ist und v_soll einführen
    ///TODO: accelaration mode einführen -> v_soll = v_max - brauche ich dann noch einen acc.mode?
    ///TODO: velocitymode einführen -> acc auf max - dann bbrauchen wir auch keinen extra mode
    //uint16_t deltaTick = acc;
}


void Motor::enable() {
    digitalWrite(enablePin,LOW);
}


void Motor::disable() {
    digitalWrite(enablePin,HIGH);
}


void Motor::reset() { 
    disable(); //disable first
    delay(1);
    digitalWrite(rstPin,LOW);
    delay(50);
    digitalWrite(rstPin,HIGH);
}