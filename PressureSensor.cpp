#include "PressureSensor.h"
#include "LogConsole.h"
#include "Utils.h"


/*
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le capteur
  @param readsCountPerValue Nombre de lecture analogique à chaque mise à jour de la valeur
 */
PressureSensor::PressureSensor(const String& name,
                               uint8_t       pin,
                               uint8_t       readsCountPerValue /* = 10 */) :
  AnalogSensor(name, Utils::GetStringFromDict(PRESSURE), Utils::GetStringFromDict(PRESSURE_UNIT), pin, readsCountPerValue)
{
  SetMinAndMaxVoltages(0.5, 4.5);
  SetMinAndMaxValues(0.0, 12.0);
  CalibrateWith2Measures(0.5, 0.0, 4.5, 12.0);
}


/*
  Destructeur
 */
PressureSensor::~PressureSensor()
{
}


/* 
  Initialisation du capteur
 */
void PressureSensor::Initialize()
{
  LOG_MESSAGE(F("Initialisation du capteur de pression %s sur la broche %d OK\n"),
              m_name.c_str(),
              m_pin);

  // Rien à faire pour l'initialisation
  m_sensorOK = true;
}
