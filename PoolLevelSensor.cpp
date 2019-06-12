#include "PoolLevelSensor.h"
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
PoolLevelSensor::PoolLevelSensor(const String& name,
                                 uint8_t       pinOut,
                                 uint8_t       pinMin,
                                 uint8_t       pinLow,
                                 uint8_t       pinHigh,
                                 uint8_t       pinMax) :
  LiquidLevelSensor(name, pinOut, pinMin),
  m_lowContactSensor(name + Utils::GetStringFromDict(LOW_SUFFIX), pinOut, pinLow),
  m_highContactSensor(name + Utils::GetStringFromDict(HIGH_SUFFIX), pinOut, pinHigh),
  m_maxContactSensor(name + Utils::GetStringFromDict(MAX_SUFFIX), pinOut, pinMax)
{
}


/*
  Destructeur
 */
PoolLevelSensor::~PoolLevelSensor()
{
}


/* 
  Initialisation du capteur
 */
void PoolLevelSensor::Initialize()
{
  LOG_MESSAGE(F("Initialisation du capteur de niveau piscine %s OK\n"),
              m_name.c_str(),
              m_pin);

  // Rien à faire pour l'initialisation car les capteur de contact sont initialisés individuellement

  m_sensorOK = true;
}


/** 
  Mise à jour de la valeur du capteur
  @param logValue True pour afficher un log en même temps que la mise à jour
*/
void PoolLevelSensor::Update(bool logValue)
{
  if ( m_sensorOK == true )
  {
    // Mise à jour du LiquidLevelSensor
    LiquidLevelSensor::Update(false);

    // Si pour le LiquidLevelSensor, le niveau est OK, on va tester
    // les capteurs du PoolLevelSensor
    if ( IsTooLow() == false )
    {
      m_lowContactSensor.Update(false);
      m_highContactSensor.Update(false);
      m_maxContactSensor.Update(false);

      // Stockage de la valeur en partant de la valeur max
      uint8_t level    = LEVEL_MAX;
      String  levelStr = Utils::GetStringFromDict(LEVEL_MAX_STR);
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
}
