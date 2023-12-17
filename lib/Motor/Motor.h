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

        //Variablen:
        uint8_t ms = 1; // Microstepping
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
    bool writeMicroStepping(uint8_t divider);

    /// @brief gibt Mikrostepfaktor zurück
    /// @return [1|2|4|8|16]
    uint8_t getMicroStepping() {
        return ms;
    }

    /// @brief setzt die Richtung des Motors im Motortreiber
    /// @param directionFlag Richtung [0|1]
    void writeDirection(bool directionFlag);

    /// @brief schaltet den Motor einen Tick weiter
    void writeTick(bool directionFlag);


    /// @brief enables the driver at driver pin "Enable"
    void enable();

    /// @brief Disables the driver at driver pin "Enable"
    void disable();

    /// @brief resets the motor driver
    void reset();
};

#endif