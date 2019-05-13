#ifndef __WATER_LEVEL_SENSOR_H__
#define __WATER_LEVEL_SENSOR_H__

#include "ContactSensor.h"


/*
  @enum WaterLevel
  @brief Définition des niveaux d'eau
*/
enum WaterLevel
{
  LEVEL_MAX,
  LEVEL_HIGH,
  LEVEL_OK,
  LEVEL_LOW,
  LEVEL_MIN
};


/*
  @class WaterLevelSensor
  @brief Classe de gestion de sondes de niveau d'eau
*/
class WaterLevelSensor : public Sensor
{
private:

protected:
  ContactSensor  m_maxContactSensor;              // Capteur de niveau max
  ContactSensor  m_highContactSensor;             // Capteur de niveau correct haut
  ContactSensor  m_lowContactSensor;              // Capteur de niveau correct bas
  ContactSensor  m_minContactSensor;              // Capteur de niveau min

public:
  // Constructeur
  WaterLevelSensor(const String& name,
                   uint8_t       pinOut,
                   uint8_t       pinMax,
                   uint8_t       pinHigh,
                   uint8_t       pinLow,
                   uint8_t       pinMin);

  // Destructeur
  virtual ~WaterLevelSensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire le niveau d'eau
  WaterLevel GetLevel() const
    { return (WaterLevel) GetValue(0); }

  // Mise à jour de la valeur du capteur
  virtual void Update(bool logValue);
};

#endif  // __WATER_LEVEL_SENSOR_H__
