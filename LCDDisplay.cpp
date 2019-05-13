#include "LCDDisplay.h"
#include "LogConsole.h"

#include <Arduino.h>


static const uint8_t s_heartData[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

static const uint8_t s_emptyHeartData[8] = {
  0b00000,
  0b01010,
  0b10101,
  0b10001,
  0b10001,
  0b01010,
  0b00100,
  0b00000
};

static const uint8_t s_smileyData[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

static const uint8_t s_frownieData[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};

static const uint8_t s_armsDownData[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};

static const uint8_t s_armsUpData[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


DisplayMode::DisplayMode(DisplayFunction displayFunction,
                         unsigned int    displayPeriodInMs) :
  m_displayFunction(displayFunction),
  m_displayPeriodInMs(displayPeriodInMs)
{
}


DisplayMode::~DisplayMode()
{
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


/* 
  Initialiser l'afficheur de type I2C
*/
void LCDDisplayClass::Initialize(uint8_t address,
                                 uint8_t cols,
                                 uint8_t rows)
{
  m_cols = cols;
  m_rows = rows;

  LiquidCrystal_I2C* lcdDisplay = new LiquidCrystal_I2C(address, cols, rows);
  
  if ( lcdDisplay != NULL )
  {
    lcdDisplay->init();
    lcdDisplay->backlight();

    lcdDisplay->createChar(HEART,       s_heartData);
    lcdDisplay->createChar(EMPTY_HEART, s_emptyHeartData);
    lcdDisplay->createChar(SMILEY,      s_smileyData);
    lcdDisplay->createChar(FROWNIE,     s_frownieData);
    lcdDisplay->createChar(ARMS_DOWN,   s_armsDownData);
    lcdDisplay->createChar(ARMS_UP,     s_armsUpData);

    m_display = lcdDisplay;
  }

  else
  {
    LOG_MESSAGE(F("Impossible d'initialiser l'afficheur LCD\n"));
  }
}


/* 
  Effacer l'écran
*/
void LCDDisplayClass::Clear()
{
  if ( m_display != NULL )
  {
    m_display->clear();
  }
}


/* 
  Effacer une ligne
*/
void LCDDisplayClass::ClearLine(uint8_t row)
{
  static String s_emptyLine = String(' ', m_cols);
  
  if ( m_display != NULL )
  {
    m_display->setCursor(0, row);
    m_display->print(s_emptyLine.c_str());
  }
}


#define  BUFFER_SIZE  100
char LCD_TEXT_BUFFER[BUFFER_SIZE];

/*
  Afficher une chaine
*/
void LCDDisplayClass::Print(uint8_t     col,
                            uint8_t     row,
                            const char* format, ...)
{
  memset(LCD_TEXT_BUFFER, 0, BUFFER_SIZE);

  if ( m_display != NULL )
  {
    va_list argList;
    va_start(argList, format);
    vsnprintf(LCD_TEXT_BUFFER, BUFFER_SIZE, format, argList);
    va_end(argList);

    m_display->setCursor(col, row);
    m_display->print(LCD_TEXT_BUFFER);
  }
}


/*
  Enregistrer une fonction d'affichage
*/
void LCDDisplayClass::RegisterDisplayMode(uint8_t      ID,
                                          DisplayMode* displayMode)
{
  if ( displayMode != NULL )
  {
    m_displayModes[ID] = displayMode;
  }
}


/* 
  Mettre à jour l'affichage courant
  @param displayMask True pour forcer l'affichage du masque
*/
void LCDDisplayClass::UpdateCurrentDisplay(bool displayMask)
{
  if ( m_display != NULL )
  {
    std::map< uint8_t, DisplayMode* >::iterator it = m_displayModes.find(m_currentDisplayMode);
    if ( it != m_displayModes.end() )
    {
      DisplayFunction func = it->second->m_displayFunction;
      (*func)(displayMask);
    }
  }
}


/* 
  Changer l'affichage courant
*/
void LCDDisplayClass::SetCurrentDisplayMode(uint8_t modeID)
{
  if ( m_display != NULL )
  {
    if ( m_displayModes.find(modeID) != m_displayModes.end() )
    {
      m_currentDisplayMode = modeID;

      // Après un changement de mode, on efface l'écran et on force l'affichage avec le masque
      Clear();
      UpdateCurrentDisplay(true);
    }
  }
}


/*
  Boucle de traitement à appeler dans le programme principal
  @param nowMillisec Heure actulelle en ms
*/
void LCDDisplayClass::Loop(uint32_t nowMillisec)
{
  if ( m_display != NULL )
  {
    // Traitement de l'affichage courant en tenant compte de la période
    std::map< uint8_t, DisplayMode* >::iterator it = m_displayModes.find(m_currentDisplayMode);
    if ( it != m_displayModes.end() )
    {
      DisplayFunction func = it->second->m_displayFunction;
      unsigned int period  = it->second->m_displayPeriodInMs;

      // Si la période est atteinte
      if ( nowMillisec >= m_lastRefreshTime + period )
      {
        (*func)(false);

        m_lastRefreshTime = nowMillisec;
      }
    }
      
    // Affichage d'un signal de vie toutes les secondes
    if ( nowMillisec >= m_lastHearbeatRefreshTime + 1000L )
    {
      static bool s_heartbeat = true;

      m_display->setCursor(m_cols - 1, m_rows - 1);
      m_display->write( ( s_heartbeat == true ) ? (uint8_t) HEART : (uint8_t) EMPTY_HEART );

      s_heartbeat = !s_heartbeat;

      m_lastHearbeatRefreshTime = nowMillisec;
    }
  }
}


LCDDisplayClass LCDDisplay;
