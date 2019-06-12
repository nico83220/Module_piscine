#ifndef __LIQUID_LEVEL_SENSOR_H__
#define __LIQUID_LEVEL_SENSOR_H__

#include "ContactSensor.h"

#define  LEVEL_OK   0
#define  LEVEL_MIN  1


/*
  @class LiquidLevelSensor
  @brief Classe de gestion de sondes de niveau d'un liquide
*/
class LiquidLevelSensor : public Sensor
{
private:

protected:
  ContactSensor  m_minContactSensor;              // Capteur de niveau min

public:
  // Constructeur
  LiquidLevelSensor(const String& name,
                    uint8_t       pinOut,
                    uint8_t       pinMin);

  // Destructeur
  virtual ~LiquidLevelSensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire si le niveau de liquide est trop bas
  virtual bool IsTooLow() const
    { return (GetLevel() == LEVEL_MIN); }

  // Lire le niveau de liquide
  uint8_t GetLevel() const
    { return (uint8_t) GetValue(0); }

  // Mise à jour de la valeur du capteur
  virtual void Update(bool logValue);
};

#endif  // __LIQUID_LEVEL_SENSOR_H__
