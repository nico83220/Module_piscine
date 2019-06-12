#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "ObjectBase.h"

#include <ArduinoSTL.h>
#include <vector>


/*
  @class Sensor
  @brief Classe abstraite definissant une classe générique de gestion de senseurs
*/
class Sensor : public ObjectBase
{
private:

protected:
  std::vector< float >  m_values;            // Valeurs du capteur (un même capteur peut en avoir plusieurs)
  uint8_t               m_pin;               // Broche sur laquelle est connecté le capteur
  bool                  m_sensorOK;          // Etat OK du capteur

  static std::vector< Sensor* >  m_sensors;  // Liste des capteurs du système

public:
  // Constructeur
  Sensor(const String& name,
         uint8_t       pin,
         uint8_t       valuesNumber);

  // Destructeur
  virtual ~Sensor();

  // Initialisation du capteur
  virtual void Initialize() = 0;

  // Mise à jour de la valeur du capteur
  virtual void Update(bool logValue) = 0;

  // Lecture de la valeur du capteur
  float GetValue(uint8_t whichValue = 0) const;

  // Lire si le capteur est OK
  bool IsOK() const
    { return m_sensorOK; }

  // Initialiser tous les capteurs
  static void InitializeAllSensors();

  // Mettre à jour tous les capteurs
  static void UpdateAllSensors(bool logValue);

  // Lire la valeur d'un capteur
  static float GetValue(const String& name,
                         uint8_t       whichValue = 0);
};

#endif  // __SENSOR_H__
