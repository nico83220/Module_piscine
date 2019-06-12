#ifndef __PUMP_H__
#define __PUMP_H__

#include "Actuator.h"

class LiquidLevelSensor;


/*
  @class Pump
  @brief Classe de gestion de pompes  
*/
class Pump : public Actuator
{
private:

protected:
  Relay*              m_commandRelay;                // Relais de commande de la pompe
  LiquidLevelSensor*  m_liquidLevelSensor;           // Capteur de niveau de liquide à pomper

public:
  // Constructeur
  Pump(const String&      name,
       Relay*             commandRelay,
       LiquidLevelSensor* liquidLevelSensor);

  // Destructeur
  virtual ~Pump();

  // Démarrer la pompe
  virtual void Start(uint32_t requestedRunningTimeInMs);

  // Boucle de traitement à appeler dans le programme principal
  virtual void Loop(uint32_t nowMillisec);
};

#endif  // __PUMP_H__
