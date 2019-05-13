#include "Utils.h"

#include <avr/wdt.h>
#include <avr/pgmspace.h>


/*
  Effectuer un RESET logiciel
*/
/* static */
void Utils::SoftwareReset()
{
  // Positionnement du watchdog à 15 ms
  wdt_enable(WDTO_15MS);

  // Boucle infinie pour réveiller le watchdog
  while ( 1 ) {}
}


/*
  Conversion d'un caractère UTF8 en ASCII étendue
  @param value Caractère UTF8 à convertir
  @return byte Caractère converti (0 si le caractère est ignoré)
*/
/* static */
byte Utils::UTF8ToASCII(byte value)
{
  static byte s_lastByte = 0;
  
  byte res = 0;
  
  // ASCII standard
  if ( value < 128 )   
  {
    // RAZ de dernier caractère mémorisé
    s_lastByte = 0;
    
    res = value;
  }

  else
  {
    // On récupère le caractère précédant
    byte lastByte = s_lastByte;

    // Mémorisation du caractère courant pour le prochain coup
    s_lastByte = value;

    // Suivant le 1er caractère du code UTF8 traité mors du précédant appel
    switch ( lastByte )
    {
    case 0xC2:
      res = value;
      break;

    case 0xC3:
      res = value | 0xC0;
      break;
    
    case 0x82:
      // Symbole euro
      if ( value == 0xAC )
      {
        res = 0x80;
      }
      break;

    default:
      break;
    }
  }
  
  return res;                                     
}


/*
  Conversion en place d'une chaine UTF8 en ASCII étendu (plus court)
  @param value Texte à convertir modifié en sortie
*/
/* static */
void Utils::UTF8ToASCII(char* value)
{      
  int  k = 0;
  
  for ( uint8_t i=0; i<strlen(value); ++i )
  {
    char c = UTF8ToASCII(value[i]);
    if ( c != 0 )
    {
      value[k++] = c;
    }
  }
  
  value[k] = 0;
}


#define FLOAT_TEXT_BUFFER_SIZE 25

// Constante avec les puissances de 10
const long PROGMEM POWER_OF_10[] = {0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

/* 
  Conversion double en string
  @param value Valeur à convertir
  @param precision Précision de la conversion (nombre de chiffres après la virgule)
*/
/* static */
String Utils::Float2String(float   value, 
                           uint8_t precision)
{
  String res;

  // On limite à 8 chiffres après la virgule
  if ( precision > 8 )
  {
    precision = 8;
  }

  char floatTextBuffer[FLOAT_TEXT_BUFFER_SIZE];
  memset(floatTextBuffer, 0, FLOAT_TEXT_BUFFER_SIZE);
  
  // Pointeur de construction
  char* tmpPtr = floatTextBuffer;

  // Partie entière
  long integerPart = (long) value;
  itoa(integerPart, tmpPtr, 10);
  while ( *tmpPtr != '\0' )
  {
    ++tmpPtr;
  }

  // Si on veut des chiffres après la virgule
  if ( precision > 0 )
  {
    // On met le caractère '.'
    *tmpPtr = '.';
  
    // On passe à la partie décimale
    ++tmpPtr;
  
    long decimalPart = abs((long)((value - (float) integerPart) * pgm_read_dword(POWER_OF_10 + precision)));
    itoa(decimalPart, tmpPtr, 10);
  }
  
  res = String(floatTextBuffer);
  
  return res;
}


/*
  Lire la mémoire RAM disponible
 */
/* static */
int Utils::GetFreeRAM()
{
  extern int  __heap_start;
  extern int* __brkval; 
  
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


// Buffer pour la conversion depuis la RAM
#define  RAM_BUFFER_SIZE  256

/* 
  Lire une chaine depuis depuis le dictionnaire
*/
/* static */
String Utils::GetStringFromDict(int ID)
{
  String res;

  char RAMBuffer[RAM_BUFFER_SIZE];
  memset(RAMBuffer, 0, RAM_BUFFER_SIZE);
  strcpy_P(RAMBuffer, (char*) pgm_read_word(&(DICT[ID])));
  res = String(RAMBuffer);
  
  return res;
}
