#ifndef __ONE_WIRE_DALLAS_TEMPERATURE_SENSOR_H__
#define __ONE_WIRE_DALLAS_TEMPERATURE_SENSOR_H__

#include "Sensor.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <yasm.h>


/*
  @class OneWireDallasTemperatureSensor
  @brief Classe de gestion de sondes de température de type 1-wire
*/
class OneWireDallasTemperatureSensor : public Sensor
{
private:

protected:
  OneWire            m_oneWireSensor;
  DallasTemperature  m_temperatureSensor;
  uint8_t            m_temperatureSensorAddress[8];
  String             m_temperatureSensorAddressStr;
  YASM               m_stateMachine;
  
  // Senseur courant pour les méthodes statiques de gestion d'état
  static OneWireDallasTemperatureSensor* m_currentSensor;

  // Etat "Demande de valeur"
  static void RequestValueState();

  // Etat "Attente de valeur"
  static void WaitValueState();

  // Etat "Valeur disponible"
  static void ValueAvailableState();

public:
  // Constructeur
  OneWireDallasTemperatureSensor(const String& name,
                                 uint8_t       pin);

  // Destructeur
  virtual ~OneWireDallasTemperatureSensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire la température
  float GetTemperature() const
    { return GetValue(0); }

  // Lire l'adresse sous forme de chaine
  String GetTemperatureSensorAddress() const
    { return m_temperatureSensorAddressStr; }

  // Mise à jour de la valeur du capteur
  virtual void Update(bool logValue);
};

#endif  // __ONE_WIRE_DALLAS_TEMPERATURE_SENSOR_H__
