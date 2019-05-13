#ifndef __CURRENT_SENSOR_H__
#define __CURRENT_SENSOR_H__

#include "Sensor.h"

#include <EmonLib.h>


/*
  @class CurrentSensor
  @brief Classe de gestion de sondes de courants (pinces ampèremétriques)
*/
class CurrentSensor : public Sensor
{
private:

protected:
  uint8_t        m_readsCountPerValue;    // Nombre le lecture pour chaque mise à jour de la valeur
  EnergyMonitor  m_energyMonitor;         // Capteur de courant
  float          m_calibrationValue;      // Valeur de calibration
  
public:
  // Constructeur
  CurrentSensor(const String& name,
                uint8_t       pin,
                uint8_t       readsCountPerValue = 1480,
                float         calibrationValue   = 111.1);

  // Destructeur
  virtual ~CurrentSensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire le courant
  float GetCurrent() const
    { return GetValue(0); }

  // Lire la puissance
  float GetPower() const
    { return GetCurrent() * 230.0; }

  // Mise à jour de la valeur du capteur
  virtual void Update(bool logValue);
};

#endif  // __CURRENT_SENSOR_H__


