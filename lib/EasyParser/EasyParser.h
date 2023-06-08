/*
 * Class EasyParser provides a parsed string with different substrings identified
 * It is chosen form the InputString-Lib of the Astrolab-Project - and modified
 * Changes to previous version: 
 *    ENTER is not a valid "String"
 *    numbers are initially set to 0 instead of -1, so cmd! = cmd!.0 = cmd!.0.0 = cmd!.0.0.0
 * 
 * Gunter Wiedemann
 * 18.05.2023
 *
 */

//don't forget the endif-Statement
#ifndef EASYPARSER_H
#define EASYPARSER_H

#include <Arduino.h>


/// @brief Parses the read string
class EasyParser
{

private:

  void ParseWholeString(void);
  // eine Deklaration wie "String validKeyWord[]" und anschließender Definition im Konstruktor geht leider nicht
  // deswegen werden die Keywords hier mit definiert 
  int nKeyWords = 13; //sizeof liefert Anzahl aller Zeichen (56 tbc)
  String validKeyWord[13] = {"help","ver","macc","mvel","micro", "calacc", "calgyro"\
                             "imu", "imuraw", "imuoffset", "iacc", "igyro", "test"};
  String wholeString = "";
  bool InKeyWord(String);

public:

  EasyParser();
  void ClearString();
  void SetString(String);
  bool stringIsValid;  //zeigt an, ob der wholeString gültig ist
  String cmd;
  bool set; //set=0 -> ? set=1 ->!
  bool get; //get=0 -> ! get=1 ->?
  int number[3]; //enthaelt bis zu 3 Werte danach - brauchen wir hier nicht (nur einen)
  void printHelp();
};



#endif
