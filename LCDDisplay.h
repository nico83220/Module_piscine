#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

#include "ObjectBase.h"

#include <ArduinoSTL.h>
#include <LiquidCrystal_I2C.h>
#include <map>


enum SpecialCharID
{
  HEART        = 0,
  EMPTY_HEART  = 1,
  SMILEY       = 2,
  FROWNIE      = 3,
  ARMS_DOWN    = 4,
  ARMS_UP      = 5,
};


// Type "fonction d'affichage"
typedef void (*DisplayFunction)(bool);


/*
  @class DisplayMode
  @brief Classe permettant de définir des affichages
*/
class DisplayMode
{
  friend class LCDDisplayClass;
  
private:

protected:
  DisplayFunction  m_displayFunction;    // Fonction d'affichage
  unsigned int     m_displayPeriodInMs;  // Période de publication

public:
  DisplayMode(DisplayFunction displayFunction,
              unsigned int    displayPeriodInMs);

  virtual ~DisplayMode();
};


/*
  @class LCDDisplayClass
  @brief Classe de gestion de mon afficheur LCD
*/
class LCDDisplayClass
{
private:

protected:
  // Afficheur LCD
  LiquidCrystal_I2C*  m_display;
  
  // Nombre de colonnes
  uint8_t  m_cols;

  // Nombre de lignes
  uint8_t  m_rows;
  
  // Identifiant du mode d'affichage courant
  uint8_t  m_currentDisplayMode;

  // Modes d'affichage d'écrans
  std::map< uint8_t, DisplayMode* >  m_displayModes;

  // Heure de dernière mise à jour de l'affichage
  uint32_t m_lastRefreshTime;

  // Heure de dernière mise à jour de lu heartbeat
  uint32_t m_lastHearbeatRefreshTime;

public:
  // Initialiser l'afficheur de type I2C
  void Initialize(uint8_t address,
                  uint8_t cols,
                  uint8_t rows);

  // Effacer l'écran
  void Clear();

  // Effacer une ligne
  void ClearLine(uint8_t row);
  
  // Afficher une chaine
  void Print(uint8_t     col,
             uint8_t     row,
             const char* format, ...);

  // Enregistrer un mode d'affichage
  void RegisterDisplayMode(uint8_t      ID,
                           DisplayMode* displayMode);

  // Mettre à jour l'affichage courant
  void UpdateCurrentDisplay(bool displayMask);

  // Changer l'affichage courant
  void SetCurrentDisplayMode(uint8_t modeID);

  // Boucle de traitement à appeler dans le programme principal
  void Loop(uint32_t nowMillisec);
};


extern LCDDisplayClass LCDDisplay;

#endif  // __LCD_DISPLAY_H__
