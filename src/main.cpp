#include <Arduino.h>
#include <IMU.h>
#include "Wire.h" 
#include <EasyParser.h>
#include <Motor.h>
#include <constants.h>
#include <DueTimer.h>
#include <controller.h>
#include <math.h>

#define DEBUG 1

#if DEBUG == 1
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
#else
  #define debug(x) 
  #define debugln(x) 
#endif

IMU imu;

Motor motor; // = Motor(); TODO: muss der Konstruktor von Motor aufgerufen werden

int stepTime = 250;
int16_t temp = -1;
int testsum = 0;
unsigned long  lastTickTime, currentTime, nextSampleTime, nextTickTime, newNextTickTime, time1, time2, time3, deltatime1, deltatime2;
bool nextDir = false; // Richtung des Motors beim nächsten step
int testx = 0;  // Auslesewert für accx der imu

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

/// @brief  berechnet verschiedene statistische Parameter von IMUX
void calcStatistics() {
  //1000 Werte von IMUX
  int n = 500;
  int16_t accxs[n];
  int32_t meanValue = 0;
  int16_t minValue = 32767;
  int16_t maxValue = -32768;
  int32_t stdDev = 0;

  for(int i=0;i<n;i++) {
    //Serial.println("Wert ermitteln ...");
    accxs[i] = imu.getVal(0x3B);
    meanValue += accxs[i];
    minValue = min(minValue, accxs[i]);
    maxValue = max(maxValue, accxs[i]);
    delayMicroseconds(500);
  }
  meanValue = meanValue / n;

  // Staandardabweichung berechnen
  for(int i=1;i<n;i++) {
    stdDev += (accxs[i] - meanValue) * (accxs[i] - meanValue);
  }
  stdDev = stdDev / (n-1);
  stdDev = sqrt(stdDev);

  // Ausgabe der Werte
  Serial.print("Mittelwert: "); Serial.println(meanValue);
  Serial.print("Minwert: "); Serial.println(minValue);
  Serial.print("Abweichung nach unten: "); Serial.println(meanValue - minValue);
  Serial.print("Maxwert: "); Serial.println(maxValue);
  Serial.print("Abweichung nach oben: "); Serial.println(maxValue - meanValue);
  Serial.print("Standardabweichung: "); Serial.println(stdDev);
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
                  
    } else if ((zerlegterString.cmd == "imu") && zerlegterString.get) {
      time1 = micros();  
      imu.getAllVals();
      time2 = micros(); 
      imu.printVals();
      deltatime1 = time2 - time1;
      Serial.print("I2C-Auslesezeit: "); Serial.print(deltatime1); Serial.println(" us");

    } else if ((zerlegterString.cmd == "imux") && zerlegterString.get) {
      time1 = micros();  
      testx = imu.getVal(0x3B);
      time2 = micros(); 
      Serial.print("AccX = "); Serial.println(testx);
      deltatime1 = time2 - time1;
      Serial.print("I2C-Auslesezeit für X-Wert: "); Serial.print(deltatime1); Serial.println(" us");
        
    } else if ((zerlegterString.cmd == "iacc") && zerlegterString.set) {
      imu.setAccRange((int16_t)(zerlegterString.number[0]));
      Serial.print("Gesetzter Acc-Wert = ");
      Serial.println(imu.getMaxGState());
        
    } else if ((zerlegterString.cmd == "iacc") && zerlegterString.get) {
      Serial.print("Gesetzter Acc-Wert = ");
      Serial.println(imu.getMaxGState());

    } else if ((zerlegterString.cmd == "igyro") && zerlegterString.set) {
      imu.setGyroRange((int16_t)(zerlegterString.number[0]));
      Serial.print("Gesetzter Gyro-Wert = ");
      Serial.println(imu.getMaxDpsState());
        
    } else if ((zerlegterString.cmd == "igyro") && zerlegterString.get) {
      Serial.print("Gesetzter Gyro-Wert = ");
      Serial.println(imu.getMaxDpsState());
    
    } else if ((zerlegterString.cmd == "DAC0") && zerlegterString.set) {
      Serial.print("Gesetzter DAC0-Wert = ");
      Serial.println((zerlegterString.number[0]));
      analogWrite(DAC1,zerlegterString.number[0]); //Achtung: Wir schreiben DAC1, obwohl DAC0 kommandiert ist

    } else if ((zerlegterString.cmd == "test") && zerlegterString.get) {
      Serial.println("Statistik wird berechnet ...");
      calcStatistics(); 
      
    } else {
      //sollte nie auftreten, da immer cmd mindestens immer help enthält
      Serial.println("Message nicht verstanden.");
      zerlegterString.printHelp();
    }
  }
}



void setup() {

  /*
  motor.init(DIR, STEP, RST, MS2, MS1, MS0, ENA);
  motor.reset();
  motor.enable();
*/
  
  Timer3.attachInterrupt(stepHandler);
	Timer3.start(stepTime); // Calls every 1000us
  

  // DAC 0 und 1 als Motorausgabe setzen
  analogWriteResolution(12);

  /*
  pinMode(LED_BUILTIN,OUTPUT);
  
  digitalWrite(LED_BUILTIN,LOW);
  pinMode(18,INPUT);
  attachInterrupt(digitalPinToInterrupt(18), ledHandler, RISING);
  */

  Serial.begin(9600);
  
  Wire.begin();
  Wire.setClock(200000);
  imu.reset();

  /*
  imu.calibrateGyro();
  imu.calibrateACC(0,-16384);
  imu.calibrateACC(1,0);
  imu.calibrateACC(2,0);
  imu.printOffsets();
  */

  /*
  status.failure = false;
  status.offset = 0;
  status.setpoint = 100;
  status.state = 0;
  initController();
  */

}


void loop() {

  
  serialInteraction();


}
