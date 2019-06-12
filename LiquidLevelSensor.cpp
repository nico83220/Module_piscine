#include "LiquidLevelSensor.h"
#include "LogConsole.h"
#include "Utils.h"

#include <Arduino.h>


/*
  Constructeur
  @param name Nom du capteur
  @param pinOut Broche commune en sortie sur laquelle sont connectées les capteurs
  @param pinMin  Broche sur laquelle est connecté le capteur de niveau min
 */
LiquidLevelSensor::LiquidLevelSensor(const String& name,
                                     uint8_t       pinOut,
                                     uint8_t       pinMin) :
  Sensor(name, 255, 1),
  m_minContactSensor(name + Utils::GetStringFromDict(MIN_SUFFIX), pinOut, pinMin)
{
}


/*
  Destructeur
 */
LiquidLevelSensor::~LiquidLevelSensor()
{
}


/* 
  Initialisation du capteur
 */
void LiquidLevelSensor::Initialize()
{
  LOG_MESSAGE(F("Initialisation du capteur de niveau de liquide %s OK\n"),
                m_name.c_str(),
                m_pin);

  // Rien à faire pour l'initialisation car les capteur de contact sont initialisés individuellement

  m_sensorOK = true;
}


/** 
  Mise à jour de la valeur du capteur
  @param logValue True pour afficher un log en même temps que la mise à jour
*/
void LiquidLevelSensor::Update(bool logValue)
{
  if ( m_sensorOK == true )
  {
    m_minContactSensor.Update(false);

    // Stockage de la valeur
    uint8_t level    = LEVEL_OK;
    String  levelStr = Utils::GetStringFromDict(LEVEL_OK_STR);
    if ( m_minContactSensor.GetContact() == CONTACT_OPENED )
    {
      level    = LEVEL_MIN;
      levelStr = Utils::GetStringFromDict(LEVEL_MIN_STR);
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
