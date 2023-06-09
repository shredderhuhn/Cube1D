/*
 * class Motor configures the stepper driver (4988 tbc)
 * later, other drivers shall be added 
 *
 * Gunter Wiedemann
 *
 * 18.05.2023  initial version
 *
 *
 */

//don't forget the endif-Statement
#ifndef MOTOR_H
#define MOTOR_H

#define MAXACC 40.0         // Maximale Beschleunigung U/s^2 
#define MAXVELO 5.0         // Maximale Geschwindigkeit U/s
#define MOTOR_STEPS 200     // Steps für eine Umdrehung (Änderung oder 1-0)

#include <Arduino.h>


class Motor {
    private:
        // Pins
        uint8_t dirPin;
        uint8_t stepPin;
        uint8_t ms2Pin;
        uint8_t ms1Pin;
        uint8_t ms0Pin;
        uint8_t rstPin;
        uint8_t enablePin; // low = enabled for 4988

        //Konstanten
        int16_t maxVelo = (int16_t)(MAXVELO * 2.0 * PI * 1000.0); //Maxgeschw. in mrad/s
        int16_t maxAcc = (int16_t)(MAXACC * 2.0 * PI * 100.0); //Maxbeschl. in 10mrad/s^2

        //Variablen:
        int8_t dir = 0; // Richtung [-1|1]
        int16_t velo = 0; // Geschwindigkeit [LSB 1mrad/s], dh. +-32rad/s?
        int16_t acc = 0; // Beschleunigung [LSB 10mrad/s^2], d.h. +-320rad/s^2 oder 50U/s^2
        uint8_t ms = 1; // Microstepping
        uint16_t deltaT = 1000; //Abtastzeit in us
        uint16_t stepTick = 0; //Umschaltung 0/1 bzw. 1/0 des stepSignal in us
        bool enableFlag = false;



    public:

    // kein Konstrukor, da die Pin-Ausgangswerte (HIGH/LOW), 
    // die außerhalb von setup gesetzt werden, wieder alle auf LOW gesetzt werden
    // Deswegen wird alles in einer init-Funktion gesetzt, die in setup ausgeführt wird

    /// @brief Konstruktor, der alle Pins setzt, aber den Motor nicht enabled
    /// @param _dirPin Richtungspin (wird 0 gesetzt)
    /// @param _stepPin muss getoggelt werden, damit der Motor sich dreht
    /// @param _rstPin Reset muss 1 sein für Betrieb (wird 1 gesetzt)
    /// @param _ms2Pin Microstepping MSB
    /// @param _ms1Pin Microstepping mittleres Bit
    /// @param _ms0Pin Microstepping LSB
    /// @param _enablePin Enable-Pin, muss 0 sein für Betrieb (wird aber auf 1 gesetzt)
        void init(  uint8_t _dirPin, 
                uint8_t _stepPin, 
                uint8_t _rstPin, 
                uint8_t _ms2Pin, uint8_t _ms1Pin, uint8_t _ms0Pin, 
                uint8_t _enablePin);

    /// @brief setzt die Mikrosteps auf einen bestimmten Faktor (1/divider) (für 4988), 
    /// @param divider [1|2|4|8|16]
    /// @parblock	    
    /// MS0     MS1	    MS2	    Microstep Resolution
    /// Low	    Low	    Low	    Full step
    /// High	Low	    Low	    Half step
    /// Low	    High	Low	    Quarter step
    /// High	High	Low	    Eighth step
    /// High	High	High	Sixteenth step
    /// @return setzen hat geklappt (richtiger Faktor angegeben)
    bool setMicroStepping(uint8_t divider);

    /// @brief gibt Mikrostepfaktor zurück
    /// @return [1|2|4|8|16]
    uint8_t getMicroStepping() {
        return ms;
    }

    /// @brief setzt die Richtung des Motors im Motortreiber
    /// @param directionFlag Richtung [-1|1]
    void setDirection(uint8_t directionFlag);

    /// @brief gebt Richtung des Motors zurück
    /// @return Richtung [-1|1]
    int8_t getDirection() {
        return dir;
    }

    /// @brief setzt die Geschwindigkeit als Variable (nicht den Timer)
    /// @param velo Geschwindigkeit in [LSB 1mrad/s], dh. +-32rad/s?
    void setVelocity(int16_t _velo);

    /// @brief gibt die Geschwindigkeit zurück 
    /// @return 
    int16_t getVelocity() {
        return velo;
    }

    /// @brief setzt die Beschleunigung als Variable (nicht den Timer)
    /// @param velo Geschwindigkeit in [LSB 1mrad/s], dh. +-32rad/s?
    void setAcceleration(int16_t _acc);

    /// @brief gibt die Geschwindigkeit zurück 
    /// @return 
    int16_t getAccelaration() {
        return acc;
    }

    /// @brief setzt die Abtastzeit zur Berechnung der Interruptzeit
    /// @param time Abtastzeit
    void setDeltaT(uint16_t time);

    /// @brief gibt die Abtastzeit zurück (deltaT)
    /// @return deltaT in us
    uint16_t getDeltaT() {
        return deltaT;
    }

    /// @brief setzt die Schaltgeschwindigkeit des Stepsignals in Abhängigkeit von acc
    /// @param tick 
    void setStepTick(uint16_t tick);

    /// @brief Schaltgeschwindigkeit des Stepsignals in us
    /// @return stepTick in us
    uint16_t getStepTick() {
        return stepTick;
    }

    /// @brief enables the driver at driver pin "Enable"
    void enable();

    /// @brief Disables the driver at driver pin "Enable"
    void disable();

    /// @brief resets the motor driver
    void reset();
};

#endif