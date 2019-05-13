#ifndef __PRESSURE_SENSOR_H__
#define __PRESSURE_SENSOR_H__

#include "AnalogSensor.h"


/*
  @class PressureSensor
  @brief Classe de gestion de sondes de pression
*/
class PressureSensor : public AnalogSensor
{
private:

protected:
  
public:
  // Constructeur
  PressureSensor(const String& name,
                 uint8_t       pin,
                 uint8_t       readsCountPerValue = 10);

  // Destructeur
  virtual ~PressureSensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire la pression
  float GetPressure() const
    { return GetValue(0); }
};

#endif  // __PRESSURE_SENSOR_H__


