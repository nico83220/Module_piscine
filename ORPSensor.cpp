#include "ORPSensor.h"
#include "LogConsole.h"
#include "Utils.h"


/*
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le capteur
  @param readsCountPerValue Nombre de lecture analogique à chaque mise à jour de la valeur
 */
ORPSensor::ORPSensor(const String& name,
                     uint8_t       pin,
                     uint8_t       readsCountPerValue /* = 10 */) :
  AnalogSensor(name, Utils::GetStringFromDict(ORP), Utils::GetStringFromDict(ORP_UNIT), pin, readsCountPerValue)
{
  m_a = -1000 / 1.037;
  m_b =  2500 / 1.037;
  m_calibrated = true;
}


/*
  Destructeur
 */
ORPSensor::~ORPSensor()
{
}


/* 
  Initialisation du capteur
 */
void ORPSensor::Initialize()
{
  LOG_MESSAGE(F("Initialisation du capteur d'ORP %s sur la broche %d OK\n"),
              m_name.c_str(),
              m_pin);

  // Rien à faire pour l'initialisation
  m_sensorOK = true;
}
