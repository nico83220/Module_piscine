#ifndef __ORP_SENSOR_H__
#define __ORP_SENSOR_H__

#include "AnalogSensor.h"


/*
  @class ORPSensor
  @brief Classe de gestion de sondes d'ORP
*/
class ORPSensor : public AnalogSensor
{
private:

protected:
  
public:
  // Constructeur
  ORPSensor(const String& name,
           uint8_t       pin,
           uint8_t       readsCountPerValue = 10);

  // Destructeur
  virtual ~ORPSensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire l'ORP
  float GetORP() const
    { return GetValue(0); }
};

#endif  // __ORP_SENSOR_H__


