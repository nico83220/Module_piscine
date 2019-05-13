#include "WaterLevelSensor.h"
#include "LogConsole.h"
#include "Utils.h"

#include <Arduino.h>


/*
  Constructeur
  @param name Nom du capteur
  @param pinOut Broche commune en sortie sur laquelle sont connectées les capteurs
  @param pinMax Broche sur laquelle est connecté le capteur de niveau max
  @param pinHigh  Broche sur laquelle est connecté le capteur de niveau haut
  @param pinLow   Broche sur laquelle est connecté le capteur de niveau bas
  @param pinMin  Broche sur laquelle est connecté le capteur de niveau min
 */
WaterLevelSensor::WaterLevelSensor(const String& name,
                                   uint8_t       pinOut,
                                   uint8_t       pinMax,
                                   uint8_t       pinHigh,
                                   uint8_t       pinLow,
                                   uint8_t       pinMin) :
  Sensor(name, 255, 1),
  m_maxContactSensor(name + Utils::GetStringFromDict(MAX_SUFFIX), pinOut, pinMax),
  m_highContactSensor(name + Utils::GetStringFromDict(HIGH_SUFFIX), pinOut, pinHigh),
  m_lowContactSensor(name + Utils::GetStringFromDict(LOW_SUFFIX), pinOut, pinLow),
  m_minContactSensor(name + Utils::GetStringFromDict(MIN_SUFFIX), pinOut, pinMin)
{
}


/*
  Destructeur
 */
WaterLevelSensor::~WaterLevelSensor()
{
}


/* 
  Initialisation du capteur
 */
void WaterLevelSensor::Initialize()
{
  LOG_MESSAGE(F("Initialisation du capteur de niveau d'eau %s OK\n"),
              m_name.c_str(),
              m_pin);

  // Rien à faire pour l'initialisation car les capteur de contact sont initialisés individuellement

  m_sensorOK = true;
}


/** 
  Mise à jour de la valeur du capteur
  @param logValue True pour afficher un log en même temps que la mise à jour
*/
void WaterLevelSensor::Update(bool logValue)
{
  if ( m_sensorOK == true )
  {
    m_maxContactSensor.Update(false);
    m_highContactSensor.Update(false);
    m_lowContactSensor.Update(false);
    m_minContactSensor.Update(false);

    // Stockage de la valeur
    WaterLevel level    = LEVEL_MAX;
    String     levelStr = Utils::GetStringFromDict(LEVEL_MAX_STR);
    if ( m_maxContactSensor.GetContact() == CONTACT_OPENED )
    {
      level    = LEVEL_HIGH;
      levelStr = Utils::GetStringFromDict(LEVEL_HIGH_STR);
      if ( m_highContactSensor.GetContact() == CONTACT_OPENED )
      {
        level    = LEVEL_OK;
        levelStr = Utils::GetStringFromDict(LEVEL_OK_STR);
        if ( m_lowContactSensor.GetContact() == CONTACT_OPENED )
        {
          level    = LEVEL_LOW;
          levelStr = Utils::GetStringFromDict(LEVEL_LOW_STR);
          if ( m_minContactSensor.GetContact() == CONTACT_OPENED )
          {
            level    = LEVEL_MIN;
            levelStr = Utils::GetStringFromDict(LEVEL_MIN_STR);
          }
        }
      }
    }
    m_values[0] = level;
  
    if ( logValue == true )
    {
      LOG_MESSAGE(F("Capteur %s - niveau d'eau : %s\n"),
                  m_name.c_str(),
                  levelStr.c_str());
    }
  }
}
