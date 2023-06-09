#include <Arduino.h>
#include <IMU.h>
#include "Wire.h" 
#include <EasyParser.h>
#include <Motor.h>
#include <constants.h>
#include<DueTimer.h>

// using a 200-step motor (most common)
#define MOTOR_STEPS 200
// configure the pins connected
/*
#define DIR                         9
#define STEP                        8
#define RST                         7   // needs to be HIGH to work
#define MS2                         6
#define MS1                         5
#define MS0                         4
#define ENA                         3   // HIGH = stop
*/


IMU imu;

Motor motor; // = Motor(); ???

int stepTime = 1000;
int16_t temp = -1;
int testsum = 0;
unsigned long  time1, time2, time3, deltatime1, deltatime2;

// Methods and vars for printing
char result[7]; // temporary variable used in convert function
char* toStr(int16_t character) { // converts int16 to string and formatting
  sprintf(result, "%6d", character);
  return result;
}

// schaltet die steps 
void stepHandler() {
  static bool level = true;
  level = !level;
  digitalWrite(STEP, level);
}

void serialInteraction() {
  static EasyParser zerlegterString;
  
    if (Serial.available()){        // Daten liegen an
    
    String msg = Serial.readString(); // Nachricht lesen
    Serial.print("Empfangene Message: ");
    Serial.println(msg);
    zerlegterString.ClearString();
    zerlegterString.SetString(msg); 
    
    // Switch-Anweisung funktioniert nur mit Integer, daher if-Anweisung
    if(zerlegterString.cmd == "help") {
      zerlegterString.printHelp();
      
    } else if (zerlegterString.cmd == "ver") {
      Serial.print("Version vom: ");
      Serial.print(__DATE__);
      Serial.print("  ");
      Serial.println(__TIME__);
      
    } else if (zerlegterString.cmd == "macc" && zerlegterString.set) {
      motor.setAcceleration((int16_t)(zerlegterString.number[0]));

    } else if (zerlegterString.cmd == "macc" && zerlegterString.get) {
      Serial.print("macc = ");
      Serial.println(motor.getAccelaration());
  
    } else if (zerlegterString.cmd == "mvel" && zerlegterString.set) {
      motor.setVelocity((int16_t)(zerlegterString.number[0]));

    } else if (zerlegterString.cmd == "mvel" && zerlegterString.get) {
      Serial.print("mvel = ");
      Serial.println(motor.getVelocity());
      
    } else if ((zerlegterString.cmd == "micro") && zerlegterString.set) {
      motor.setMicroStepping((uint8_t)(zerlegterString.number[0]));
      Serial.print("Neues Microstepping = 1/");
      Serial.println(motor.getMicroStepping());
      
    } else if ((zerlegterString.cmd == "micro") && zerlegterString.get) {
      Serial.print("Microstepping = 1/");
      Serial.println(motor.getMicroStepping());
      
    } else if ((zerlegterString.cmd == "imu") && zerlegterString.get) {
      imu.getAllVals();
      imu.printVals();

    } else if ((zerlegterString.cmd == "imuraw") && zerlegterString.get) {
      imu.printRawVals();
      
    } else if ((zerlegterString.cmd == "imuoffset") && zerlegterString.get) {
      int accx = 1;
      for(int i = 1; i<imu.getMaxGState(); i++) {
        accx *= 2;
      }
      accx = -16384/accx;
      Serial.print("Kalbirationswert = ");
      Serial.println(accx);
      imu.calibrateGyro();
      imu.calibrateACC(0,accx);
      imu.calibrateACC(1,0);
      imu.calibrateACC(2,0);
      Serial.println("Neue Offsets: ");
      imu.printOffsets();
        
    } else if ((zerlegterString.cmd == "iacc") && zerlegterString.set) {
      imu.setAccRange((int16_t)(zerlegterString.number[0]));
      Serial.print("Gesetzter Acc-Wert = ");
      Serial.print(imu.getMaxGState());
        
    } else if ((zerlegterString.cmd == "iacc") && zerlegterString.get) {
      Serial.print("Gesetzter Acc-Wert = ");
      Serial.print(imu.getMaxGState());

    } else if ((zerlegterString.cmd == "igyro") && zerlegterString.set) {
      imu.setGyroRange((int16_t)(zerlegterString.number[0]));
      Serial.print("Gesetzter Gyro-Wert = ");
      Serial.print(imu.getMaxDpsState());
        
    } else if ((zerlegterString.cmd == "igyro") && zerlegterString.get) {
      Serial.print("Gesetzter Gyro-Wert = ");
      Serial.print(imu.getMaxDpsState());
    
    } else if ((zerlegterString.cmd == "test") && zerlegterString.set) {
        Serial.println("Kein Test definiert");
       
    } else {
      //sollte nie auftreten, da immer cmd mindestens immer help enthält
      Serial.println("Message nicht verstanden.");
      zerlegterString.printHelp();
    }
  }
}



void setup() {

  

  motor.init(DIR, STEP, RST, MS2, MS1, MS0, ENA);
  motor.reset();
  motor.enable();

  Timer3.attachInterrupt(stepHandler);
	Timer3.start(stepTime); // Calls every 50ms

  //pinMode(LED_BUILTIN,OUTPUT);

  Serial.begin(9600);
  
  Wire.begin();
  imu.reset();
  imu.calibrateGyro();
  imu.calibrateACC(0,-16384);
  imu.calibrateACC(1,0);
  imu.calibrateACC(2,0);
  imu.printOffsets();
  
}


void loop() {
  serialInteraction();
  
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
  
}
