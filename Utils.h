#ifndef __UTILS_H__
#define __UTILS_H__

#include "Dictionary.h"

#include <Arduino.h>


/*
  @class Utils
  @brief Classe de fonctions utilitaires
*/
class Utils
{
private:

protected:

public:
  // Effectuer un RESET logiciel
  static void SoftwareReset();

  // Conversion d'un caractère UTF8 en ASCII étendue
  static byte UTF8ToASCII(byte value);

  // Conversion en place d'une chaine UTF8 en ASCII étendu (plus court)
  static void UTF8ToASCII(char* value);

  // Conversion double en string
  static String Float2String(float   value, 
                             uint8_t precision);

  // Conversion entier en string
  static String Int2String(int    value, 
                           int8_t charsNumber = -1,
                           char   padChar     = '0');

  // Lire la mémoire RAM disponible
  static int GetFreeRAM();

  // Lire une chaine depuis le dictionnaire
  static String GetStringFromDict(int ID);

  // Ecrire une chaine en EEPROM
  static void UpdateStringInEEPROM(int&          address,
                                   const String& value);

  // Lire une chaine en EEPROM
  static String ReadStringInEEPROM(int& address);
};


#ifndef htons
#define htons(x) ( ((x)<< 8 & 0xFF00) | \
                   ((x)>> 8 & 0x00FF) )
#endif

#ifndef ntohs
#define ntohs(x) htons(x)
#endif

#ifndef htonl
#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#endif

#ifndef ntohl
#define ntohl(x) htonl(x)
#endif

#endif  // __UTILS_H__
