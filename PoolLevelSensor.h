#ifndef __POOL_LEVEL_SENSOR_H__
#define __POOL_LEVEL_SENSOR_H__

#include "LiquidLevelSensor.h"


#define  LEVEL_LOW   2
#define  LEVEL_HIGH  3
#define  LEVEL_MAX   4


/*
  @class PoolLevelSensor
  @brief Classe de gestion de sondes de niveau d'eau
*/
class PoolLevelSensor : public LiquidLevelSensor
{
private:

protected:
  ContactSensor  m_lowContactSensor;              // Capteur de niveau correct bas
  ContactSensor  m_highContactSensor;             // Capteur de niveau correct haut
  ContactSensor  m_maxContactSensor;              // Capteur de niveau max

public:
  // Constructeur
  PoolLevelSensor(const String& name,
                  uint8_t       pinOut,
                  uint8_t       pinMin,
                  uint8_t       pinLow,
                  uint8_t       pinHigh,
                  uint8_t       pinMax);

  // Destructeur
  virtual ~PoolLevelSensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire si le niveau de liquide est trop bas
  virtual bool IsTooLow() const
    { return ( (LiquidLevelSensor::IsTooLow() == true) || (GetLevel() == LEVEL_LOW) ); }

  // Mise à jour de la valeur du capteur
  virtual void Update(bool logValue);
};

#endif  // __POOL_LEVEL_SENSOR_H__
