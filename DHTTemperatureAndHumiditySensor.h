#ifndef __DHT_TEMPERATURE_AND_HUMIDITY_SENSOR_H__
#define __DHT_TEMPERATURE_AND_HUMIDITY_SENSOR_H__

#include "Sensor.h"

#include <DHT.h>


/*
  @class DHTTemperatureAndHumiditySensor
  @brief Classe de gestion de sondes de température et humidité de type DHT
*/
class DHTTemperatureAndHumiditySensor : public Sensor
{
private:

protected:
  DHT  m_temperatureAndHumiditySensor;
  
public:
  // Constructeur
  DHTTemperatureAndHumiditySensor(const String& name,
                                  uint8_t       pin,
                                  uint8_t       type);

  // Destructeur
  virtual ~DHTTemperatureAndHumiditySensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire la température
  float GetTemperature() const
    { return GetValue(0); }

  // Lire l'humidité
  float GetHumidity() const
    { return GetValue(1); }

  // Mise à jour de la valeur du capteur
  virtual void Update(bool logValue);
};


#endif  // __DHT_TEMPERATURE_AND_HUMIDITY_SENSOR_H__


