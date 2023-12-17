#include <Arduino.h>
#include <IMU.h>
#include "Wire.h" 
#include <EasyParser.h>
#include <Motor.h>
#include <constants.h>
#include <DueTimer.h>
#include <controller.h>

IMU imu;

Motor motor; // = Motor(); TODO: muss der Konstruktor von Motor aufgerufen werden

int stepTime = 1000;
int16_t temp = -1;
int testsum = 0;
unsigned long  lastTickTime, currentTime, nextSampleTime, nextTickTime, newNextTickTime, time1, time2, time3, deltatime1, deltatime2;
bool nextDir = false; // Richtung des Motors beim nächsten step

// Methods and vars for printing
char result[7]; // temporary variable used in convert function
char* toStr(int16_t character) { // converts int16 to string and formatting
  sprintf(result, "%6d", character);
  return result;
}

// schaltet die steps 
void stepHandler() { 
  static bool level = false;
  digitalWrite(DIR, nextDir);
  digitalWrite(STEP, level);
  level = !level;  
}

void ledHandler() {
  static bool level = true;
  digitalWrite(LED_BUILTIN, level);
  level = !level;
}

void tickHandler() {
  motor.writeTick(nextDir);
  lastTickTime = micros();
  nextTickTime = lastTickTime + ctrl.tick;
  Timer3.stop();
  Timer3.start(ctrl.tick);
  nextDir = ctrl.dir;
}

void controllerHandler() {
  imu.getAllVals();
  calcController(imu.getAccX());
  //calcController(0);
  calcTick();
  currentTime = micros();
  nextSampleTime = currentTime + SAMPLETIME;
  newNextTickTime = lastTickTime + ctrl.tick;
  //nextTickTime is calced in tickHandler()
  if (nextTickTime > nextSampleTime) { 
    nextDir = ctrl.dir; // unklar, ob so sinnvoll oder ob man es einfach weglassen kann (einflusss sollte gering sein)   
    if (newNextTickTime < currentTime) {
      // neuer Tick liegt in der Vergangenheit
      tickHandler();
    } else {
      // neuer Tick liegt in der Zukunft
      Timer3.stop();
      Timer3.start(newNextTickTime - currentTime);
      nextTickTime = newNextTickTime;
    }
  }
}


/*
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
*/


void setup() {

  
  motor.init(DIR, STEP, RST, MS2, MS1, MS0, ENA);
  motor.reset();
  motor.enable();

  Timer3.attachInterrupt(stepHandler);
	Timer3.start(stepTime); // Calls every 50ms

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  //pinMode(107,OUTPUT);
  //digitalWrite(107,LOW);
  //pinMode(103,OUTPUT);
  //digitalWrite(103,HIGH);
  pinMode(18,INPUT);
  attachInterrupt(digitalPinToInterrupt(18), ledHandler, RISING);

  Serial.begin(9600);
  
  Wire.begin();
  Wire.setClock(1600000);
  imu.reset();
  imu.calibrateGyro();
  imu.calibrateACC(0,-16384);
  imu.calibrateACC(1,0);
  imu.calibrateACC(2,0);
  imu.printOffsets();

  status.failure = false;
  status.offset = 0;
  status.setpoint = 100;
  status.state = 0;
  initController();
  
}


void loop() {

  /*
  serialInteraction();
  
  
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
*/
  //Serial.print(digitalRead(18));
  //if (digitalRead(18) == HIGH) {ledHandler();}
  //delay(1000);

  for (int i=0; i<6; i++) {
    calcController(0);
    calcTick();
    Serial.print("ctrl.u = ");
    Serial.println(ctrl.u);
    Serial.print("ctrl.v = ");
    Serial.println(ctrl.v);
    Serial.print("ctrl.tick = ");
    Serial.println(ctrl.tick);
  }
  
while(1);

}
