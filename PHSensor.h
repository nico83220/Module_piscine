#ifndef __PH_SENSOR_H__
#define __PH_SENSOR_H__

#include "AnalogSensor.h"


/*
  @class PHSensor
  @brief Classe de gestion de sondes de pH
*/
class PHSensor : public AnalogSensor
{
private:

protected:
  // Mettre à jour la droite de transfert en fonction de la valeur lue
  virtual void UpdateTransferParameters(float value);
 
public:
  // Constructeur
  PHSensor(const String& name,
           uint8_t       pin,
           uint8_t       readsCountPerValue     = 10,
           Sensor*       waterTemperatureSensor = NULL);

  // Destructeur
  virtual ~PHSensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire le pH
  float GetPH() const
    { return GetValue(0); }
};

#endif  // __PH_SENSOR_H__


