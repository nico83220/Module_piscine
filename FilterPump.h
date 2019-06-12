#ifndef __FILTER_PUMP_H__
#define __FILTER_PUMP_H__

#include "Pump.h"
#include "Config.h"

class PressureSensor;


/*
  @class FilterPump
  @brief Classe de gestion de pompes de filtration  
*/
class FilterPump : public Pump,
                   public ISerializable
{
private:

protected:
  PressureSensor*  m_filterPressureSensor;            // Capteur de pression du filtre
  float            m_minPressure;                     // Pression minimum du filtre
  float            m_maxPressure;                     // Pression maximum du filtre

public:
  // Constructeur
  FilterPump(const String&      name,
             Relay*             commandRelay,
             LiquidLevelSensor* liquidLevelSensor,
             PressureSensor*    filterPressureSensor);

  // Destructeur
  virtual ~FilterPump();

  // Changer la pression minimum du filtre
  void SetMinPressure(float minPressure)
    { m_minPressure = minPressure; }

  // Lire la pression minimum du filtre
  float GetMinPressure() const
    { return m_minPressure; }

  // Changer la pression maximum du filtre
  void SetMaxPressure(float maxPressure)
    { m_maxPressure = maxPressure; }

  // Lire la pression maximum du filtre
  float GetMaxPressure() const
    { return m_maxPressure; }

  // Démarrer la pompe
  virtual void Start(uint32_t requestedRunningTimeInMs);

  // Boucle de traitement à appeler dans le programme principal
  virtual void Loop(uint32_t nowMillisec);

  // Sérialiser l'objet vers ou depuis l'EEPROM
  virtual void Serialize(const SerializeMode& serializeMode,
                         int&                 EEPROMAddress);
};

#endif  // __FILTER_PUMP_H__
