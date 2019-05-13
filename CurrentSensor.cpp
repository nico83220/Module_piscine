#include "CurrentSensor.h"
#include "LogConsole.h"
#include "Utils.h"


/*
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le capteur
  @param readsCountPerValue Nombre de lecture analogique à chaque mise à jour de la valeur (1480 par défaut pris dans l'exemple de EmonLib)
  @param calibrationValue Valeur de calibration pour la mesure (111.1 par défaut pris dans l'exemple de EmonLib)
 */
CurrentSensor::CurrentSensor(const String& name,
                             uint8_t       pin,
                             uint8_t       readsCountPerValue /* = 1480 */,
                             float         calibrationValue   /* = 111.1 */) :
  Sensor(name, pin, 1),
  m_readsCountPerValue(readsCountPerValue),
  m_energyMonitor(),
  m_calibrationValue(calibrationValue)
{
}


/*
  Destructeur
 */
CurrentSensor::~CurrentSensor()
{
}


/* 
  Initialisation du capteur
 */
void CurrentSensor::Initialize()
{
  // Configuration en mesure de courant avec une valeur de calibration
  m_energyMonitor.current(m_pin, m_calibrationValue);

  LOG_MESSAGE(F("Initialisation du capteur de courant %s sur la broche %d OK\n"),
              m_name.c_str(),
              m_pin);

  m_sensorOK = true;
}


/** 
  Mise à jour de la valeur du capteur
  @param logValue True pour afficher un log en même temps que la mise à jour
*/
void CurrentSensor::Update(bool logValue)
{
  if ( m_sensorOK == true )
  {
    // Calcul de la valeur Irms
    float current = m_energyMonitor.calcIrms(m_readsCountPerValue);

    m_values[0] = current;

    if ( logValue == true )
    {
      LOG_MESSAGE(F("Capteur %s - intensite : %s A\n"),
                  m_name.c_str(),
                  Utils::Float2String(current, 1).c_str());
    }
  }
}
