#include <EasyParser.h>    //muss hier eingefügt werden, findet .h sonst nicht
#include <Arduino.h>


EasyParser::EasyParser()
{
  ClearString(); 
}

void EasyParser::ClearString(void)
{
  stringIsValid = false;  //zeigt an, ob der wholeString gültig ist
  wholeString = "";
  cmd = "";
  set = 0; //set=0 -> ? set=1 ->!
  get = 0; //get=0 -> ! get=1 ->?
  number[0] = 0; //only valid for date! and time!
  number[1] = 0;
  number[2] = 0;
  
}

void EasyParser::SetString(String aString)
{
  ClearString();
  wholeString = aString;
  ParseWholeString();
}

void EasyParser::ParseWholeString(void)
{
  //zuerst get oder set
  String restOfString;
  int delimiterIndex = wholeString.indexOf('!'); //set?
  if (delimiterIndex >= 0) //set?
  {
    set = true;
    get = false;
    //Kommando rausloesen
    cmd = wholeString.substring(0, delimiterIndex);
    restOfString = wholeString.substring(delimiterIndex + 1); // To the end of the string

    //restlichen String auf Eingaben untersuchen
    //zunaechst ersetzen wir ggf. Doppelpunkte durch Einzelpunkte,
    //damit wir später nur einen Delimiter haben
    restOfString.replace(":", ".");
    int i = 0;
    while (i < 3) // max. Anzahl der durch Delimiters getrennte Groessen
    {
      delimiterIndex = restOfString.indexOf(".");
      if (delimiterIndex > -1)
      {
        number[i] = restOfString.substring(0,delimiterIndex).toInt();
        restOfString = restOfString.substring(delimiterIndex+1);
        i++;
      } 
      else
      {
        //if (isDigit(restOfString[0]) && (restOfString.length() > 0))
        if (isDigit(restOfString[0]))
        {
          number[i] = restOfString.toInt();
        }
        i = 3;
      }
    }
  } 
  else //get?
  {
    delimiterIndex = wholeString.indexOf('?'); //get wird automatisch angenommen
    set = false;
    get = true;
    cmd = wholeString.substring(0, delimiterIndex);
  }
  //Wenn kein Keywort identifiziert wurde, dann Hilfe ausgeben
  if (InKeyWord(cmd) == false) cmd = "help";
  stringIsValid = true;
}

bool EasyParser::InKeyWord(String aString)
{
 for (int i = 0; i < nKeyWords; i++)
  {
    if (aString.equals(validKeyWord[i])) return true;
  }
  return false;
}

void EasyParser::printHelp() {
  Serial.println("Folgende Kommandos stehen zur Verfügung.");
  Serial.println("?               - Hilfe");
  Serial.println("help?           - Hilfe");
  Serial.println("ver?            - Version der Software: Datum und Zeit");
  Serial.println("imu?            - IMU-Werte ausgeben");
  Serial.println("imuraw?         - IMU-Rohwerte ausgeben");
  Serial.println("imucal?         - IMU-Offset-Werte kalibrieren und ausgeben (z=-1g");
  Serial.println("iacc!x          - IMU-Beschl.Range einstellen x = [0|1|2|3] => 2g|4g|8g|16g");
  Serial.println("iacc?           - IMU-Beschl.Range auslesen x = [0|1|2|3] => 2g|4g|8g|16g");
  Serial.println("igyro!x         - IMU-Gyro-Range einstellen x = [0|1|2|3] => 250|500|1000|2000 dps");
  Serial.println("igyro?          - IMU-Gyro-Range auslesen x = [0|1|2|3] => 250|500|1000|2000 dps");  
  Serial.println("DAC1!x          - DAC1 schreiben x = [0..4095]");  
  Serial.println("testx?          - IMUx wird ausgelesen (einige 100x) und dann statistisch bewertet (TA unbekannt).");
  Serial.println("samplex?        - IMUx wird 1000x ausgelesen und dann statistisch bewertet (TA=500us).");
  Serial.println("tp!step.n       - test des Tiefpasses: Sprung nach step, n Iterationen");
  Serial.println("");
  Serial.println("Für alle Eingaben gilt: ON|VALID = 1 sowie OFF|INVALID = 0");
  
}
